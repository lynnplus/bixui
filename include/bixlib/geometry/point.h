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

/**
 * @file point.h
 * @brief 2D point (x and y) type definitions.
 */

#pragma once

#include <bixlib/export_macro.h>
#include <bixlib/utils/concepts.h>
#include <bixlib/utils/numeric.h>

#include <algorithm>

namespace bix {
namespace geom {

/**
 * A template structure representing a point in 2D space.
 *
 * This class provides a basic representation of a coordinate pair (x, y).
 * It is used as a building block for more complex geometric shapes like lines and rectangles.
 * @tparam T The numeric type of the coordinates. Must satisfy the Real concept (signed arithmetic).
 */
template <Real T>
struct BIX_PUBLIC PointT {
    T x{0}; ///< The horizontal coordinate, initialized to 0 by default.
    T y{0}; ///< The vertical coordinate, initialized to 0 by default.

    /** Default constructor. Initializes coordinates to zero. */
    constexpr PointT() noexcept = default;

    /** Constructor with explicit x and y values. */
    constexpr PointT(T x_, T y_) noexcept : x(x_), y(y_) {}

    PointT(const PointT& other) = default;
    PointT& operator=(const PointT& other) = default;

    /**
     * Constructs a PointT from another compatible coordinate type.
     *
     * This constructor is constrained by the Upcastable concept, allowing only
     * non-narrowing conversions (e.g., PointT<int> to PointT<float>).
     * It is marked as explicit to prevent accidental implicit type promotion
     * in complex geometric expressions.
     *
     * @tparam From The source coordinate type.
     * @param other The source point to convert from.
     */
    template <typename From>
    requires Upcastable<T, From>
    constexpr explicit PointT(const PointT<From>& other) noexcept
        : x(static_cast<T>(other.x))
        , y(static_cast<T>(other.y)) {}

    /**
     * Assigns from another compatible numeric type.
     *
     * Only available if the source type From can be safely upcasted to T.
     *
     * @tparam From The source numeric type.
     * @param other The source object to assign from.
     * @return A reference to this instance.
     */
    template <typename From>
    requires Upcastable<T, From>
    constexpr PointT& operator=(const PointT<From>& other) noexcept {
        x = static_cast<T>(other.x);
        y = static_cast<T>(other.y);
        return *this;
    }

    /**
     * Creates a PointT from another PointT of a different type.
     *
     * @note If converting from a floating-point type to an integral type,
     * this method performs **truncation** (fractional parts are discarded).
     * @tparam U The source coordinate type.
     * @param other The source point to convert from.
     * @return A new PointT with coordinates cast to type T.
     */
    template <typename U>
    static constexpr PointT from(const PointT<U>& other) noexcept {
        return PointT(static_cast<T>(other.x), static_cast<T>(other.y));
    }

    /**
     * Creates a PointT from a floating-point PointT using rounding.
     *
     * If the target type T is integral, coordinates are rounded to the nearest integer.
     * If the target type T is floating-point, this behaves like #from.
     * @tparam U The source floating-point type.
     * @param other The source point.
     * @return A new PointT with rounded coordinates.
     */
    template <std::floating_point U>
    static constexpr PointT roundFrom(const PointT<U>& other) noexcept {
        if constexpr (std::is_integral_v<T>) {
            return {math::round_cast<T>(other.x), math::round_cast<T>(other.y)};
        } else {
            return from(other);
        }
    }

    /**
     * Converts the current point to another numeric type.
     *
     * @tparam To The target numeric type.
     * @warning Side effect: converting from floating-point to integer will
     * truncate the fractional part. Converting between different precisions
     * (e.g., double to float) may result in data loss.
     * @return A new PointT converted to type To.
     */
    template <typename To>
    constexpr PointT<To> as() const noexcept {
        return PointT<To>::from(*this);
    }

    /**
     * Checks if the point is at the origin (0, 0).
     *
     * For floating-point types, a fuzzy comparison is used with a given epsilon.
     * For integral types, an exact comparison to zero is performed.
     * @param epsilon The precision threshold for floating-point comparison.
     * @return True if both coordinates are zero (within epsilon for floats).
     */
    constexpr bool isZero(T epsilon = math::default_eps_v<T>) const noexcept {
        if constexpr (std::is_floating_point_v<T>) {
            return math::fuzzyIsZero(x, epsilon) && math::fuzzyIsZero(y, epsilon);
        } else {
            BIX_UNUSED(epsilon)
            return x == 0 && y == 0;
        }
    }

    /**
     * Compares two points for approximate equality.
     *
     * This is the recommended method for comparing floating-point points to
     * mitigate precision issues.
     * @param other The point to compare with.
     * @param epsilon The tolerance threshold.
     * @return True if the distance between coordinates is within epsilon.
     */
    constexpr bool equals(const PointT& other, T epsilon = math::default_eps_v<T>) const noexcept {
        if constexpr (std::is_floating_point_v<T>) {
            return math::fuzzyEqual(x, other.x, epsilon) && math::fuzzyEqual(y, other.y, epsilon);
        } else {
            BIX_UNUSED(epsilon)
            return x == other.x && y == other.y;
        }
    }

    /**
     * Calculates the Euclidean distance to another point.
     *
     * @param other The target point to measure distance to.
     * @return The distance as a double.
     */
    double distanceTo(const PointT& other) const noexcept {
        // TODO impl template<T>
        T dx = x - other.x;
        T dy = y - other.y;
        return std::sqrt(static_cast<double>(dx * dx + dy * dy));
    }

    /**
     * Returns a point clamped within the specified rectangular bounds.
     *
     * @param min The minimum boundary point.
     * @param max The maximum boundary point.
     * @return A new PointT constrained within [min, max].
     */
    PointT clamped(const PointT& min, const PointT& max) const noexcept {
        return {std::clamp(x, min.x, max.x), std::clamp(y, min.y, max.y)};
    }

    /**
     * Rounds the coordinates to the nearest integers.
     *
     * @note This method creates a new PointT with integer types.
     * @warning This operation involves a rounding side effect: precision is lost
     * as fractional parts are removed.
     * @return A new PointT<int> with rounded values.
     * @see math::round_cast
     */
    PointT<int> round() const noexcept { return {math::round_cast<int>(x), math::round_cast<int>(y)}; }

    /** @name Arithmetic Operators
     * @{
     */
    /**
     * Strict equality operator.
     *
     * Performs a zero-tolerance comparison using math::exactlyEqual.
     * @note For floating-point types, use equals() for robust logical comparison.
     */
    constexpr bool operator==(const PointT& rhs) const noexcept {
        return math::exactlyEqual(x, rhs.x) && math::exactlyEqual(y, rhs.y);
    }

    constexpr PointT operator+(const PointT& rhs) const noexcept { return {x + rhs.x, y + rhs.y}; }

    constexpr PointT operator-(const PointT& rhs) const noexcept { return {x - rhs.x, y - rhs.y}; }

    PointT& operator+=(const PointT& rhs) noexcept {
        x += rhs.x;
        y += rhs.y;
        return *this;
    }

    PointT& operator-=(const PointT& rhs) noexcept {
        x -= rhs.x;
        y -= rhs.y;
        return *this;
    }

    constexpr PointT operator*(T factor) const noexcept { return {x * factor, y * factor}; }

    template <std::floating_point F>
    requires(!std::same_as<T, F>)
    constexpr PointT<std::common_type_t<T, F>> operator*(F factor) const noexcept {
        using ResultT = std::common_type_t<T, F>;
        return PointT<ResultT>{
            static_cast<ResultT>(x) * static_cast<ResultT>(factor),
            static_cast<ResultT>(y) * static_cast<ResultT>(factor)
        };
    }

    ///@}
};

} // namespace geom

/** Alias for a point with integer coordinates. */
using PointI = geom::PointT<int>;
/** Alias for a point with float coordinates. */
using PointF = geom::PointT<float>;
/** Default point type used by the library (defaults to float). */
using Point = PointF;

} // namespace bix