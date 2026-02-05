// protected_object.hpp - Thread-safe wrapper (Ada protected object equivalent)
// Template for Ada to C++ migration
//
// Usage:
//   class Buffer : public embedded::concurrency::ProtectedObject<BufferState> {
//       // Protected procedures and functions
//   };
//
#pragma once

#include <condition_variable>
#include <functional>
#include <mutex>
#include <optional>
#include <shared_mutex>

namespace embedded::concurrency {

// ============================================================================
// Protected Object Base Class
// ============================================================================

/// @brief Base class for Ada-style protected objects
///
/// Provides:
/// - `procedure()` - Exclusive write access (Ada protected procedure)
/// - `function()` - Shared read access (Ada protected function)
/// - `entry()` - Conditional entry with barrier (Ada protected entry)
///
/// @tparam State The internal state type to protect
template <typename State>
class ProtectedObject {
protected:
    mutable std::shared_mutex mutex_;
    mutable std::condition_variable_any cv_;
    State state_;
    
public:
    /// @brief Construct with default-initialized state
    ProtectedObject() = default;
    
    /// @brief Construct with arguments forwarded to State constructor
    template <typename... Args>
    explicit ProtectedObject(Args&&... args)
        : state_{std::forward<Args>(args)...}
    {}
    
    // Non-copyable (like Ada protected objects)
    ProtectedObject(const ProtectedObject&) = delete;
    ProtectedObject& operator=(const ProtectedObject&) = delete;
    
    /// @brief Execute a protected procedure (exclusive access)
    ///
    /// Ada equivalent:
    /// ```ada
    /// protected Buffer is
    ///    procedure Put(Data : in Sensor_Data);
    /// ```
    ///
    /// C++ usage:
    /// ```cpp
    /// buffer.procedure([](State& s) { s.data = new_data; });
    /// ```
    template <typename Func>
    auto procedure(Func&& fn) -> decltype(fn(state_)) {
        std::unique_lock lock{mutex_};
        return fn(state_);
    }
    
    /// @brief Execute a protected function (shared/read access)
    ///
    /// Ada equivalent:
    /// ```ada
    /// protected Buffer is
    ///    function Empty return Boolean;
    /// ```
    ///
    /// C++ usage:
    /// ```cpp
    /// bool empty = buffer.function([](const State& s) { return s.empty(); });
    /// ```
    template <typename Func>
    auto function(Func&& fn) const -> decltype(fn(state_)) {
        std::shared_lock lock{mutex_};
        return fn(state_);
    }
    
    /// @brief Execute a protected entry with barrier (blocking)
    ///
    /// Ada equivalent:
    /// ```ada
    /// protected Buffer is
    ///    entry Get(Data : out Sensor_Data);
    /// private
    ///    Has_Data : Boolean := False;
    /// end Buffer;
    ///
    /// entry Get(Data : out Sensor_Data) when Has_Data is
    /// begin
    ///    Data := Store;
    ///    Has_Data := False;
    /// end Get;
    /// ```
    ///
    /// C++ usage:
    /// ```cpp
    /// auto data = buffer.entry(
    ///     [](const State& s) { return s.has_data; },  // Barrier
    ///     [](State& s) {                              // Action
    ///         auto d = s.data;
    ///         s.has_data = false;
    ///         return d;
    ///     }
    /// );
    /// ```
    template <typename Barrier, typename Func>
    auto entry(Barrier&& barrier, Func&& fn) -> decltype(fn(state_)) {
        std::unique_lock lock{mutex_};
        cv_.wait(lock, [&] { return barrier(state_); });
        return fn(state_);
    }
    
    /// @brief Non-blocking entry attempt
    ///
    /// @return std::nullopt if barrier not satisfied, otherwise result of fn
    template <typename Barrier, typename Func>
    auto try_entry(Barrier&& barrier, Func&& fn) 
        -> std::optional<decltype(fn(state_))> 
    {
        std::unique_lock lock{mutex_};
        if (!barrier(state_)) {
            return std::nullopt;
        }
        return fn(state_);
    }
    
    /// @brief Timed entry with timeout
    ///
    /// @return std::nullopt if timeout or barrier not satisfied
    template <typename Barrier, typename Func, typename Rep, typename Period>
    auto timed_entry(
        Barrier&& barrier, 
        Func&& fn,
        std::chrono::duration<Rep, Period> timeout
    ) -> std::optional<decltype(fn(state_))> 
    {
        std::unique_lock lock{mutex_};
        if (!cv_.wait_for(lock, timeout, [&] { return barrier(state_); })) {
            return std::nullopt;  // Timeout
        }
        return fn(state_);
    }
    
    /// @brief Notify waiting entries that state may have changed
    void notify_one() {
        cv_.notify_one();
    }
    
    /// @brief Notify all waiting entries that state may have changed
    void notify_all() {
        cv_.notify_all();
    }
};

// ============================================================================
// Monitor Pattern (Simpler alternative)
// ============================================================================

/// @brief Simple monitor with automatic locking (for less complex cases)
template <typename T>
class Monitor {
    mutable std::mutex mutex_;
    T value_;
    
public:
    Monitor() = default;
    
    template <typename... Args>
    explicit Monitor(Args&&... args) : value_{std::forward<Args>(args)...} {}
    
    /// @brief Execute function with locked access to value
    template <typename Func>
    auto with(Func&& fn) -> decltype(fn(value_)) {
        std::lock_guard lock{mutex_};
        return fn(value_);
    }
    
    /// @brief Execute function with locked const access
    template <typename Func>
    auto with(Func&& fn) const -> decltype(fn(value_)) {
        std::lock_guard lock{mutex_};
        return fn(value_);
    }
    
    /// @brief Get a copy of the value (thread-safe)
    [[nodiscard]] T get() const {
        std::lock_guard lock{mutex_};
        return value_;
    }
    
    /// @brief Set the value (thread-safe)
    void set(T new_value) {
        std::lock_guard lock{mutex_};
        value_ = std::move(new_value);
    }
};

// ============================================================================
// Read-Write Monitor (Multiple readers, single writer)
// ============================================================================

/// @brief Monitor optimized for read-heavy workloads
template <typename T>
class RwMonitor {
    mutable std::shared_mutex mutex_;
    T value_;
    
public:
    RwMonitor() = default;
    
    template <typename... Args>
    explicit RwMonitor(Args&&... args) : value_{std::forward<Args>(args)...} {}
    
    /// @brief Execute function with exclusive write access
    template <typename Func>
    auto write(Func&& fn) -> decltype(fn(value_)) {
        std::unique_lock lock{mutex_};
        return fn(value_);
    }
    
    /// @brief Execute function with shared read access
    template <typename Func>
    auto read(Func&& fn) const -> decltype(fn(value_)) {
        std::shared_lock lock{mutex_};
        return fn(value_);
    }
    
    /// @brief Get a copy of the value (shared lock)
    [[nodiscard]] T get() const {
        std::shared_lock lock{mutex_};
        return value_;
    }
    
    /// @brief Set the value (exclusive lock)
    void set(T new_value) {
        std::unique_lock lock{mutex_};
        value_ = std::move(new_value);
    }
};

} // namespace embedded::concurrency
