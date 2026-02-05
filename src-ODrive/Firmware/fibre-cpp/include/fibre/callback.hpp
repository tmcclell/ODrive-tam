#ifndef __CALLBACK_HPP
#define __CALLBACK_HPP

#include <stdlib.h>
#include <typeinfo>
#include <tuple>
#include <functional>
#include <type_traits>

/**
 * @file callback.hpp
 * @brief Provides a lightweight callback mechanism for the fibre library.
 * 
 * This header defines a type-erased callback class that can wrap function pointers,
 * lambdas, and member functions. It provides a uniform interface for storing and
 * invoking callbacks without requiring virtual functions or std::function.
 * 
 * @namespace fibre
 * @namespace fibre::detail
 * 
 * @class Callback
 * @brief A type-erased callback wrapper that stores a function pointer and context.
 * 
 * @tparam TRet The return type of the callback
 * @tparam TArgs The argument types of the callback
 * 
 * The Callback class provides:
 * - Construction from raw function pointers with a void* context
 * - Construction from functors (lambdas, function objects) - note: functor must outlive the Callback
 * - Safe invocation with default return value when callback is null
 * - One-shot invocation via invoke_and_clear()
 * 
 * @struct function_traits
 * @brief Helper struct to extract return type, object type, and argument types from member function pointers.
 * 
 * @struct MemberCallback
 * @brief Helper struct to create Callback objects from member function pointers.
 * 
 * @fn make_callback
 * @brief Creates a Callback from a member function pointer and object instance.
 * @tparam T The type of the member function pointer
 * @tparam func The member function pointer value
 * @param obj Pointer to the object instance
 * @return A Callback that will invoke the member function on the given object
 * 
 * @def MEMBER_CB
 * @brief Convenience macro to create a callback from an object pointer and member function name.
 * @param obj Pointer to the object instance
 * @param func Name of the member function (without parentheses or &)
 * 
 * Example usage:
 * @code
 * class MyClass {
 *     void myMethod(int x) { ... }
 * };
 * MyClass obj;
 * auto cb = MEMBER_CB(&obj, myMethod);
 * cb.invoke(42);
 * @endcode
 */
namespace fibre {

namespace detail {
template<typename T> struct get_default { static T val() { return {}; } };
template<> struct get_default<void> { static void val() {} };
}

template<typename TRet, typename ... TArgs>
class Callback {
public:
    Callback() : cb_(nullptr), ctx_(nullptr) {}
    Callback(std::nullptr_t) : cb_(nullptr), ctx_(nullptr) {}
    Callback(TRet(*callback)(void*, TArgs...), void* ctx) :
        cb_(callback), ctx_(ctx) {}

    /**
     * @brief Creates a copy of another Callback instance.
     * 
     * This is only works it the other callback has identical template parameters.
     * This constructor is templated so that construction from an incompatible
     * callback gives a useful error message.
     */
    //template<typename TRetOther, typename ... TArgsOther>
    //Callback(const Callback<TRetOther, TArgsOther...>& other) : cb_(other.cb_), ctx_(other.ctx_) {
    //    static_assert(std::is_same<Callback<TRetOther, TArgsOther...>, Callback>::value, "incompatible callback type");
    //}

    Callback(const Callback& other) : cb_(other.cb_), ctx_(other.ctx_) {}

    // If you get a compile error "[...] invokes a deleted function" that points
    // here then you're probably trying to assign a Callback with incompatible
    // template arguments to another Callback.
    template<typename TRetOther, typename ... TArgsOther>
    Callback(const Callback<TRetOther, TArgsOther...>& other) = delete;

    /**
     * @brief Constructs a callback object from a functor. The functor must
     * remain allocated throughout the lifetime of the Callback.
     */
    template<typename TFunc>
    Callback(const TFunc& func) :
        cb_([](void* ctx, TArgs...args){
            return (*(const TFunc*)ctx)(args...);
        }), ctx_((void*)&func) {}

    operator bool() {
        return cb_;
    }
    
    TRet invoke(TArgs ... arg) const {
        if (cb_) {
            return (*cb_)(ctx_, arg...);
        }
        return detail::get_default<TRet>::val();
    }

    TRet invoke_and_clear(TArgs ... arg) {
        void* ctx = ctx_;
        auto cb = cb_;
        ctx_ = nullptr;
        cb_ = nullptr;
        if (cb) {
            return (*cb)(ctx, arg...);
        }
        return detail::get_default<TRet>::val();
    }

    typedef TRet(*cb_t)(void*, TArgs...);
    cb_t get_ptr() { return cb_; }
    void* get_ctx() { return ctx_; }

private:
    TRet(*cb_)(void*, TArgs...);
    void* ctx_;
};

template<typename _TRet, typename _TObj, typename ... _TArgs>
struct function_traits {
    using TRet = _TRet;
    using TArgs = std::tuple<_TArgs...>;
    using TObj = _TObj;
};

template<typename _TRet, typename _TObj, typename ... _TArgs>
function_traits<_TRet, _TObj, _TArgs...> make_function_traits(_TRet (_TObj::*)(_TArgs...)) {
    return {};
}

template<typename T1, T1 T2, typename T3, typename T4, typename T5>
struct MemberCallback;

template<typename T, T func, typename TObj, typename TRes, typename ... TArgs>
struct MemberCallback<T, func, TObj, TRes, std::tuple<TArgs...>> {
    using cb_t = Callback<TRes, TArgs...>;
    static cb_t with(TObj* obj) {
        return cb_t{[](void* obj, TArgs... arg) {
            return (((TObj*)obj)->*func)(arg...);
        }, obj};
    }
};

template<typename T, T func,
         typename TTraits = decltype(make_function_traits(func)),
         typename MemCb = MemberCallback<T, func, typename TTraits::TObj, typename TTraits::TRet, typename TTraits::TArgs>>
typename MemCb::cb_t make_callback(typename TTraits::TObj* obj) {
    return MemCb::with(obj);
}

#define MEMBER_CB(obj, func) \
    fibre::make_callback< \
        decltype(&std::remove_reference_t<decltype(*obj)>::func), \
        &std::remove_reference_t<decltype(*obj)>::func \
    >(obj)

}

#endif // __CALLBACK_HPP