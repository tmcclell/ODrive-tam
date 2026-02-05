// range_type.hpp - Range-constrained type wrapper (Ada subtype equivalent)
// Template for Ada to C++ migration
//
// Usage:
//   using AdcValue = embedded::RangeType<std::uint16_t, 0, 4095>;
//   AdcValue reading{1234};  // OK
//   AdcValue bad{5000};      // Triggers constraint error
//
#pragma once

#include <compare>
#include <concepts>
#include <cstdint>
#include <stdexcept>
#include <type_traits>

// For embedded targets without exceptions
#ifdef EMBEDDED_TARGET
extern "C" void __builtin_trap();
#endif

namespace embedded {

// ============================================================================
// Constraint Violation Handler
// ============================================================================

/// @brief Called when a range constraint is violated
/// @note In embedded mode, triggers a fault. Otherwise, throws.
template <typename T>
[[noreturn]] inline void constraint_violation(
    [[maybe_unused]] T value,
    [[maybe_unused]] T min,
    [[maybe_unused]] T max
) {
#ifdef EMBEDDED_TARGET
    // Trigger hardware fault - will be caught by Last_Chance_Handler equivalent
    __builtin_trap();
#else
    throw std::out_of_range("Value out of range constraint");
#endif
}

// ============================================================================
// Range-Constrained Type
// ============================================================================

/// @brief Type wrapper that enforces Ada-like range constraints
/// @tparam T Underlying arithmetic type
/// @tparam Min Minimum allowed value (inclusive)
/// @tparam Max Maximum allowed value (inclusive)
template <typename T, T Min, T Max>
    requires std::integral<T> || std::floating_point<T>
class RangeType {
public:
    using underlying_type = T;
    static constexpr T min_value = Min;
    static constexpr T max_value = Max;
    
    static_assert(Min <= Max, "Min must be <= Max");
    
    /// @brief Default constructor (initializes to Min)
    constexpr RangeType() noexcept : value_{Min} {}
    
    /// @brief Construct from value with range check
    /// @param val Value to wrap (must be in [Min, Max])
    constexpr explicit RangeType(T val) : value_{val} {
        validate(val);
    }
    
    /// @brief Get the underlying value
    [[nodiscard]] constexpr T get() const noexcept { return value_; }
    
    /// @brief Explicit conversion to underlying type
    [[nodiscard]] constexpr explicit operator T() const noexcept { return value_; }
    
    /// @brief Check if a value would be valid for this range
    [[nodiscard]] static constexpr bool is_valid(T val) noexcept {
        return val >= Min && val <= Max;
    }
    
    /// @brief Assignment with range check
    constexpr RangeType& operator=(T val) {
        validate(val);
        value_ = val;
        return *this;
    }
    
    /// @brief Three-way comparison
    [[nodiscard]] constexpr auto operator<=>(const RangeType&) const = default;
    
    // Arithmetic operators that maintain constraints
    
    [[nodiscard]] constexpr RangeType operator+(const RangeType& rhs) const {
        return RangeType{static_cast<T>(value_ + rhs.value_)};
    }
    
    [[nodiscard]] constexpr RangeType operator-(const RangeType& rhs) const {
        return RangeType{static_cast<T>(value_ - rhs.value_)};
    }
    
    constexpr RangeType& operator+=(const RangeType& rhs) {
        *this = *this + rhs;
        return *this;
    }
    
    constexpr RangeType& operator-=(const RangeType& rhs) {
        *this = *this - rhs;
        return *this;
    }
    
    constexpr RangeType& operator++() {
        validate(value_ + 1);
        ++value_;
        return *this;
    }
    
    constexpr RangeType operator++(int) {
        RangeType old = *this;
        ++(*this);
        return old;
    }
    
    constexpr RangeType& operator--() {
        validate(value_ - 1);
        --value_;
        return *this;
    }
    
    constexpr RangeType operator--(int) {
        RangeType old = *this;
        --(*this);
        return old;
    }
    
private:
    T value_;
    
    /// @brief Validate that a value is within the allowed range
    static constexpr void validate(T val) {
        if (val < Min || val > Max) {
            if consteval {
                // Compile-time error
                throw "Constraint violation: value out of range";
            } else {
                // Runtime handling
                constraint_violation(val, Min, Max);
            }
        }
    }
};

// ============================================================================
// Common Range Type Aliases
// ============================================================================

/// @brief 12-bit ADC value (0-4095)
using Adc12Bit = RangeType<std::uint16_t, 0, 4095>;

/// @brief Percentage (0-100)
using Percentage = RangeType<std::uint8_t, 0, 100>;

/// @brief Byte value (0-255) - mostly for documentation clarity
using ByteValue = RangeType<std::uint8_t, 0, 255>;

/// @brief Boolean as integer (0 or 1)
using BoolInt = RangeType<std::uint8_t, 0, 1>;

// ============================================================================
// Type Traits
// ============================================================================

/// @brief Check if a type is a RangeType instantiation
template <typename T>
struct is_range_type : std::false_type {};

template <typename T, T Min, T Max>
struct is_range_type<RangeType<T, Min, Max>> : std::true_type {};

template <typename T>
inline constexpr bool is_range_type_v = is_range_type<T>::value;

/// @brief Concept for range types
template <typename T>
concept RangeTypeConcept = is_range_type_v<T>;

// ============================================================================
// Saturating Arithmetic (Alternative to exceptions)
// ============================================================================

/// @brief Range type that saturates at boundaries instead of throwing
template <typename T, T Min, T Max>
    requires std::integral<T> || std::floating_point<T>
class SaturatingRangeType {
public:
    using underlying_type = T;
    static constexpr T min_value = Min;
    static constexpr T max_value = Max;
    
    constexpr SaturatingRangeType() noexcept : value_{Min} {}
    
    constexpr explicit SaturatingRangeType(T val) noexcept 
        : value_{clamp(val)} {}
    
    [[nodiscard]] constexpr T get() const noexcept { return value_; }
    [[nodiscard]] constexpr explicit operator T() const noexcept { return value_; }
    
    constexpr SaturatingRangeType& operator=(T val) noexcept {
        value_ = clamp(val);
        return *this;
    }
    
    [[nodiscard]] constexpr auto operator<=>(const SaturatingRangeType&) const = default;
    
    [[nodiscard]] constexpr SaturatingRangeType operator+(const SaturatingRangeType& rhs) const noexcept {
        return SaturatingRangeType{static_cast<T>(value_ + rhs.value_)};
    }
    
    [[nodiscard]] constexpr SaturatingRangeType operator-(const SaturatingRangeType& rhs) const noexcept {
        return SaturatingRangeType{static_cast<T>(value_ - rhs.value_)};
    }
    
private:
    T value_;
    
    [[nodiscard]] static constexpr T clamp(T val) noexcept {
        if (val < Min) return Min;
        if (val > Max) return Max;
        return val;
    }
};

} // namespace embedded
