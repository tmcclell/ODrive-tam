// strong_type.hpp - Strong type wrapper library for Ada-like type safety
// Template for Ada to C++ migration
//
// Usage:
//   struct SensorReadingTag {};
//   using SensorReading = embedded::StrongType<std::uint16_t, SensorReadingTag>;
//
#pragma once

#include <compare>
#include <concepts>
#include <type_traits>
#include <utility>

namespace embedded {

// ============================================================================
// Skills (Mix-ins) for common operations
// ============================================================================

/// @brief Enables addition operator for strong types
template <typename T>
struct Addable {
    [[nodiscard]] friend constexpr T operator+(const T& lhs, const T& rhs) noexcept {
        return T{lhs.get() + rhs.get()};
    }
    
    constexpr T& operator+=(const T& rhs) noexcept {
        auto& self = static_cast<T&>(*this);
        self = self + rhs;
        return self;
    }
};

/// @brief Enables subtraction operator for strong types
template <typename T>
struct Subtractable {
    [[nodiscard]] friend constexpr T operator-(const T& lhs, const T& rhs) noexcept {
        return T{lhs.get() - rhs.get()};
    }
    
    constexpr T& operator-=(const T& rhs) noexcept {
        auto& self = static_cast<T&>(*this);
        self = self - rhs;
        return self;
    }
};

/// @brief Enables multiplication operator for strong types
template <typename T>
struct Multiplicable {
    [[nodiscard]] friend constexpr T operator*(const T& lhs, const T& rhs) noexcept {
        return T{lhs.get() * rhs.get()};
    }
    
    // Scalar multiplication
    template <typename Scalar>
        requires std::is_arithmetic_v<Scalar>
    [[nodiscard]] friend constexpr T operator*(const T& lhs, Scalar rhs) noexcept {
        return T{static_cast<typename T::underlying_type>(lhs.get() * rhs)};
    }
    
    template <typename Scalar>
        requires std::is_arithmetic_v<Scalar>
    [[nodiscard]] friend constexpr T operator*(Scalar lhs, const T& rhs) noexcept {
        return rhs * lhs;
    }
};

/// @brief Enables division operator for strong types
template <typename T>
struct Divisible {
    [[nodiscard]] friend constexpr T operator/(const T& lhs, const T& rhs) noexcept {
        return T{lhs.get() / rhs.get()};
    }
    
    template <typename Scalar>
        requires std::is_arithmetic_v<Scalar>
    [[nodiscard]] friend constexpr T operator/(const T& lhs, Scalar rhs) noexcept {
        return T{static_cast<typename T::underlying_type>(lhs.get() / rhs)};
    }
};

/// @brief Enables increment operators
template <typename T>
struct Incrementable {
    constexpr T& operator++() noexcept {
        auto& self = static_cast<T&>(*this);
        ++self.get();
        return self;
    }
    
    constexpr T operator++(int) noexcept {
        auto& self = static_cast<T&>(*this);
        T old = self;
        ++self;
        return old;
    }
};

/// @brief Enables decrement operators
template <typename T>
struct Decrementable {
    constexpr T& operator--() noexcept {
        auto& self = static_cast<T&>(*this);
        --self.get();
        return self;
    }
    
    constexpr T operator--(int) noexcept {
        auto& self = static_cast<T&>(*this);
        T old = self;
        --self;
        return old;
    }
};

/// @brief Enables bitwise operations
template <typename T>
struct BitwiseOps {
    [[nodiscard]] friend constexpr T operator&(const T& lhs, const T& rhs) noexcept {
        return T{lhs.get() & rhs.get()};
    }
    
    [[nodiscard]] friend constexpr T operator|(const T& lhs, const T& rhs) noexcept {
        return T{lhs.get() | rhs.get()};
    }
    
    [[nodiscard]] friend constexpr T operator^(const T& lhs, const T& rhs) noexcept {
        return T{lhs.get() ^ rhs.get()};
    }
    
    [[nodiscard]] friend constexpr T operator~(const T& val) noexcept {
        return T{~val.get()};
    }
};

// ============================================================================
// Primary Strong Type Template
// ============================================================================

/// @brief Strong type wrapper that preserves type identity
/// @tparam T Underlying type
/// @tparam Tag Unique tag type for type identity
/// @tparam Skills... Mix-in classes for enabled operations
template <typename T, typename Tag, template<typename> class... Skills>
class StrongType : public Skills<StrongType<T, Tag, Skills...>>... {
public:
    using underlying_type = T;
    using tag_type = Tag;
    
    /// @brief Default constructor (value-initializes underlying type)
    constexpr StrongType() noexcept = default;
    
    /// @brief Explicit constructor from underlying type
    /// @param val Value to wrap
    constexpr explicit StrongType(T val) noexcept 
        : value_{std::move(val)} {}
    
    /// @brief Get mutable reference to underlying value
    [[nodiscard]] constexpr T& get() noexcept { return value_; }
    
    /// @brief Get const reference to underlying value
    [[nodiscard]] constexpr const T& get() const noexcept { return value_; }
    
    /// @brief Explicit conversion to underlying type
    [[nodiscard]] constexpr explicit operator T() const noexcept { return value_; }
    
    /// @brief Three-way comparison (C++20)
    [[nodiscard]] constexpr auto operator<=>(const StrongType&) const = default;
    
private:
    T value_{};
};

// ============================================================================
// Convenience Aliases
// ============================================================================

/// @brief Strong type with arithmetic operations (add, subtract, multiply, divide)
template <typename T, typename Tag>
using ArithmeticType = StrongType<T, Tag, Addable, Subtractable, Multiplicable, Divisible>;

/// @brief Strong type with increment/decrement
template <typename T, typename Tag>
using CounterType = StrongType<T, Tag, Addable, Subtractable, Incrementable, Decrementable>;

/// @brief Strong type with bitwise operations
template <typename T, typename Tag>
using BitwiseType = StrongType<T, Tag, BitwiseOps>;

// ============================================================================
// Type Traits
// ============================================================================

/// @brief Check if a type is a StrongType instantiation
template <typename T>
struct is_strong_type : std::false_type {};

template <typename T, typename Tag, template<typename> class... Skills>
struct is_strong_type<StrongType<T, Tag, Skills...>> : std::true_type {};

template <typename T>
inline constexpr bool is_strong_type_v = is_strong_type<T>::value;

/// @brief Concept for strong types
template <typename T>
concept StrongTypeConcept = is_strong_type_v<T>;

} // namespace embedded
