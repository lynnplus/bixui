/*
 * Copyright (c) 2025-2026 Lynn <lynnplus90@gmail.com>
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#pragma once

#include <bixlib/utils/concepts.h>

#include <sstream>

/**
 * @namespace bix::math
 * Fundamental mathematical utilities for the bixlib.
 *
 * This namespace contains essential tools for handling floating-point precision,
 * safe numerical casting, and common mathematical constants used across
 * geometry, rendering, and layout modules.
 */
namespace bix::math {

namespace internal {

#ifdef BIX_DISABLE_NUMERIC_CAST_CHECK
inline constexpr bool kDisableCheck = true;
#else
inline constexpr bool kDisableCheck = false;
#endif

enum class CastError : uint8_t {
    None = 0,
    IsNan,
    Overflow,
    Underflow,
    Inexact
};

template <typename To, typename From>
__attribute__((noinline)) [[noreturn]] void dispatch_cast_error(From v, CastError err) {
    std::ostringstream oss;
    oss << "numeric_cast: [" << typeid(From).name() << " -> " << typeid(To).name() << "] "
        << "value (";
    if constexpr (std::is_same_v<From, char>) {
        oss << static_cast<int>(v) << ") ";
    } else {
        oss << v << ") ";
    }
    switch (err) {
    case CastError::Overflow: oss << "overflows"; throw std::overflow_error(oss.str());
    case CastError::Underflow: oss << "underflow"; throw std::underflow_error(oss.str());
    case CastError::Inexact: oss << "precision loss"; throw std::range_error(oss.str());
    case CastError::IsNan: oss << "isnan"; throw std::range_error(oss.str());
    default: [[unlikely]] std::abort();
    }
}

template <typename T>
using standard_int_t = std::conditional_t<
    std::is_same_v<T, char>,
    // According to the performance of char on the current platform, it is mapped to the standard char type.
    std::conditional_t<std::is_signed_v<char>, signed char, unsigned char>,
    std::conditional_t<std::is_same_v<T, bool>, int, T>>;

template <typename T>
constexpr bool physical_equal(T a, T b) noexcept {
#if defined(__GNUC__) || defined(__clang__)
    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Wfloat-equal"
#endif
    return a == b;
#if defined(__GNUC__) || defined(__clang__)
    #pragma GCC diagnostic pop
#endif
}

template <typename T>
constexpr bool is_nan_safe(T v) noexcept {
    return !physical_equal(v, v);
}
} // namespace internal

/**
 * Safely casts a numeric value with range and precision checking.
 *
 * Verifies that the source value fits within the target type's range.
 * Detects overflow, underflow, NaN, and precision loss.
 * @tparam To The target arithmetic type.
 * @tparam From The source arithmetic type.
 * @param value The value to be converted.
 * @return The value converted to the target type @p To.
 * @throw std::overflow_error If value > To_max.
 * @throw std::underflow_error If value < To_lowest.
 * @throw std::range_error If precision is lost or value is NaN.
 * @note Define BIX_DISABLE_NUMERIC_CAST_CHECK to bypass all checks for maximum performance.
 * @code
 * try {
 *     double largeValue = 1e20;
 *     int n = numeric_cast<int>(largeValue); // Will throw overflow_error
 * } catch (const std::exception& e) {
 * // Handle failed conversion
 * }
 * float f = 123.f;
 * int i = numeric_cast<int>(f); // Safe conversion, returns 123
 * @endcode
 */
template <Arithmetic To, Arithmetic From>
constexpr To numeric_cast(From value) noexcept(internal::kDisableCheck) {
#ifdef BIX_DISABLE_NUMERIC_CAST_CHECK
    return static_cast<To>(value);
#else

    using ToLimit = std::numeric_limits<To>;
    using FromLimit = std::numeric_limits<From>;
    using namespace internal;

    if constexpr (std::is_same_v<From, To>) { return value; }
    // Static analysis for always-safe conversions
    constexpr bool is_always_safe = [] {
        if constexpr (std::is_same_v<From, bool> && std::is_arithmetic_v<To>) {
            return true;
        } else if constexpr (FromLimit::is_integer && ToLimit::is_integer) {
            if constexpr (FromLimit::is_signed == ToLimit::is_signed || (!FromLimit::is_signed && ToLimit::is_signed)) {
                return ToLimit::digits >= FromLimit::digits;
            }
        } else if constexpr (std::is_floating_point_v<From> && std::is_floating_point_v<To>) {
            return ToLimit::digits > FromLimit::digits;
        } else if constexpr (FromLimit::is_integer && std::is_floating_point_v<To>) {
            constexpr int from_width = FromLimit::digits + (FromLimit::is_signed ? 1 : 0);
            return from_width <= ToLimit::digits;
        }
        return false;
    }();
    if constexpr (is_always_safe) { return static_cast<To>(value); }

    auto error = CastError::None;
    if constexpr (ToLimit::is_integer && FromLimit::is_integer) {
        using SFrom = standard_int_t<From>;
        using STo = standard_int_t<To>;
        if (std::cmp_greater(static_cast<SFrom>(value), static_cast<STo>(ToLimit::max()))) error = CastError::Overflow;
        if (std::cmp_less(static_cast<SFrom>(value), static_cast<STo>(ToLimit::lowest()))) error = CastError::Underflow;
    } else if constexpr (FromLimit::is_integer && !ToLimit::is_integer) {
        // int -> float
        if constexpr (sizeof(From) > 8) { // very large integer
            if (value > static_cast<From>(ToLimit::max())) error = CastError::Overflow;
            if (value < static_cast<From>(ToLimit::lowest())) error = CastError::Underflow;
        }
    } else if constexpr (!FromLimit::is_integer && ToLimit::is_integer) {
        // float->int
        if (is_nan_safe(value)) [[unlikely]]
            error = CastError::IsNan;
        else if (value < static_cast<From>(ToLimit::lowest()))
            error = CastError::Underflow;
        else if (value >= static_cast<From>(ToLimit::max()))
            error = CastError::Overflow;
    } else {
        // double->float
        if (is_nan_safe(value)) [[unlikely]]
            error = CastError::IsNan;
        else if (value > static_cast<From>(ToLimit::max())) {
            error = CastError::Overflow;
        } else if (value < static_cast<From>(ToLimit::lowest())) {
            error = CastError::Underflow;
        }
    }

    if (error != CastError::None) [[unlikely]] { dispatch_cast_error<To>(value, error); }

    To result = static_cast<To>(value);
    if constexpr (ToLimit::is_integer && FromLimit::is_integer) { return result; }
    if (static_cast<From>(result) != value) { dispatch_cast_error<To>(value, CastError::Inexact); }
    return result;
#endif
}

/**
 * Safely converts a floating-point value to the nearest integer.
 *
 * This function rounds the source @p value to the nearest integer using the
 * "round halfway away from zero" strategy (matching std::round). The resulting
 * integer is then validated against the range of type @p To using #numeric_cast.
 *
 * @tparam To The target integer type.
 * @tparam From The source floating-point type.
 * @param value The floating-point value to be rounded and converted.
 * @return The rounded value converted to type @p To.
 * @code
 * auto r = round_cast<int>(1.4f);  // Returns 1
 * auto r1 = round_cast<int>(1.5f);  // Returns 2
 * auto r2 = round_cast<int>(-1.5f); // Returns -2
 * auto r3 = round_cast<int8_t>(200.0); // Throws std::overflow_error
 * @endcode
 */
template <typename To, typename From>
constexpr To round_cast(From value) noexcept(internal::kDisableCheck) {
    return numeric_cast<To>(std::round(value));
}

/**
 * Safely converts a value to the largest integer less than or equal to the source.
 *
 * Maps @p value to the nearest integer in the direction of negative infinity (matching std::floor).
 * The result is subsequently checked for range safety to ensure it fits into type @p To.
 *
 * @tparam To The target integer type.
 * @tparam From The source floating-point type.
 * @param value The value to be floored.
 * @return The floored value converted to type @p To.
 * @code
 * auto f1 = floor_cast<int>(1.9f);  // Returns 1
 * auto f2 = floor_cast<int>(-1.1f); // Returns -2
 * @endcode
 */
template <typename To, typename From>
constexpr To floor_cast(From value) noexcept(internal::kDisableCheck) {
    return numeric_cast<To>(std::floor(value));
}

/**
 * Safely converts a value to the smallest integer greater than or equal to the source.
 *
 * Maps @p value to the nearest integer in the direction of positive infinity (matching std::ceil).
 * Range validation is performed via @ref numeric_cast to prevent silent overflows.
 *
 * @tparam To The target integer type.
 * @tparam From The source floating-point type.
 * @param value The value to be ceiled.
 * @return The ceiled value converted to type @p To.
 * @code
 * auto c1 = ceil_cast<int>(1.1f);  // Returns 2
 * auto c2 = ceil_cast<int>(-1.9f); // Returns -1
 * @endcode
 */
template <typename To, typename From>
constexpr To ceil_cast(From value) noexcept(internal::kDisableCheck) {
    return numeric_cast<To>(std::ceil(value));
}

/**
 * Provides the default epsilon (tolerance) values for different floating-point types.
 *
 * Epsilon is used as the maximum absolute difference allowed for two values
 * to be considered "equal" or for a value to be considered "zero".
 * @tparam T The floating-point type (float, double, or long double).
 */
template <typename T>
inline constexpr T default_eps_v;

/** Default epsilon for single-precision floats (1e-5). */
template <>
inline constexpr float default_eps_v<float> = 1e-5f; // 0.00001

/** Default epsilon for double-precision floats (1e-10). */
template <>
inline constexpr double default_eps_v<double> = 1e-10;

/** Default epsilon for extended-precision floats (1e-12). */
template <>
inline constexpr long double default_eps_v<long double> = 1e-12L;

/**
 * Checks if two floating-point values are approximately equal.
 *
 * Uses an absolute difference comparison: |a - b| <= epsilon.
 * @tparam T A type satisfying the FloatType concept.
 * @param a The first value to compare.
 * @param b The second value to compare.
 * @param epsilon The tolerance threshold. Defaults to #default_eps_v<T>.
 * @return True if the absolute difference is within epsilon, false otherwise.
 * @code
 * float a = 1.0f;
 * float b = 1.000001f;
 * if (fuzzyEqual(a, b)) { // Uses default 1e-5f
 *     // Logic for equal values
 * }
 * double d1 = 0.12345678901;
 * double d2 = 0.12345678902;
 * bool eq = fuzzyEqual(d1, d2, 1e-11); // Custom epsilon
 * @endcode
 */
template <FloatType T>
constexpr bool fuzzyEqual(T a, T b, T epsilon = default_eps_v<T>) noexcept {
    if (internal::physical_equal(a, b)) [[unlikely]]
        return true;
    const T delta = a - b;
    return (delta < 0 ? -delta : delta) <= epsilon;
}

/**
 * Checks if a floating-point value is approximately zero.
 *
 * Uses an absolute comparison: |val| <= epsilon.
 * @tparam T A type satisfying the FloatType concept.
 * @param val The value to check.
 * @param epsilon The tolerance threshold. Defaults to @ref default_eps_v<T>.
 * @return True if the value is close to zero, false otherwise.
 *  @code
 * float x = 0.000001f;
 * if (fuzzyIsZero(x)) { // Returns true as x < 1e-5f
 *     x = 0.0f;
 * }
 * @endcode
 */
template <FloatType T>
constexpr bool fuzzyIsZero(T val, T epsilon = default_eps_v<T>) noexcept {
    if (internal::physical_equal(val, static_cast<T>(0))) [[unlikely]]
        return true;
    return (val < 0 ? -val : val) <= epsilon;
}

} // namespace bix::math