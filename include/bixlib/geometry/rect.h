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
 * @file rect.h
 * @brief Rectangle geometry type definitions and operations.
 */

#pragma once

#include <bixlib/geometry/point.h>
#include <bixlib/geometry/size.h>

namespace bix {
namespace geom {

/**
 * A template structure representing a rectangle in 2D space.
 *
 * Rectangles are defined by their four edges: left, top, right, and bottom.
 * This class provides various methods for geometric calculations and boundary checks.
 * @tparam T The numeric type for coordinates which must satisfy the Real concept.
 */
template <Real T>
class BIX_PUBLIC RectT {
public:
    T left{0};   ///< The coordinate of the left edge.
    T top{0};    ///< The coordinate of the top edge.
    T right{0};  ///< The coordinate of the right edge.
    T bottom{0}; ///< The coordinate of the bottom edge.

    /** Default constructor initializing all boundaries to zero. */
    constexpr RectT() = default;

    /**
     * Constructor initializing with explicit boundary values.
     *
     * @param l Left boundary.
     * @param t Top boundary.
     * @param r Right boundary.
     * @param b Bottom boundary.
     */
    constexpr RectT(T l, T t, T r, T b) noexcept : left(l), top(t), right(r), bottom(b) {}

    /**
     * Constructor using a top-left point and dimensions.
     *     * @param l Left coordinate.
     * @param t Top coordinate.
     * @param size The dimensions of the rectangle.
     */
    constexpr RectT(T l, T t, const SizeT<T>& size) noexcept
        : left(l)
        , top(t)
        , right(l + size.width)
        , bottom(t + size.height) {}

    /**
     * Constructor initializing from origin with a given size.
     *
     * @param size The dimensions of the rectangle.
     */
    constexpr explicit RectT(const SizeT<T>& size) noexcept : RectT(0, 0, size) {}

    constexpr RectT(const RectT&) noexcept = default;
    constexpr RectT& operator=(const RectT&) noexcept = default;

    /**
     * Constructs a RectT from another compatible numeric type.
     *
     * This constructor uses the Upcastable concept to ensure that only safe,
     * non-narrowing conversions (such as RectI to RectF) are performed
     * automatically. It is marked as explicit to prevent unintended type
     * promotion in complex layout or geometry calculations.
     *
     * @tparam From The source numeric type of the rectangle coordinates.
     * @param other The source rectangle to convert from.
     */
    template <typename From>
    requires Upcastable<T, From>
    constexpr explicit RectT(const RectT<From>& other) noexcept
        : left(static_cast<T>(other.left))
        , top(static_cast<T>(other.top))
        , right(static_cast<T>(other.right))
        , bottom(static_cast<T>(other.bottom)) {}

    /**
     * Assigns from another compatible rectangle type.
     *
     * Only available if the source numeric type From can be safely
     * upcasted to T without loss of precision.
     *
     * @tparam From The source numeric type.
     * @param other The source rectangle to assign from.
     * @return A reference to this instance.
     */
    template <typename From>
    requires Upcastable<T, From>
    constexpr RectT& operator=(const RectT<From>& other) noexcept {
        left = static_cast<T>(other.left);
        top = static_cast<T>(other.top);
        right = static_cast<T>(other.right);
        bottom = static_cast<T>(other.bottom);
        return *this;
    }

    /**
     * Creates a rectangle from another instance of a different numeric type.
     * @note This operation performs an explicit cast; converting from floating-point to integral types results in
     * truncation.
     * @tparam U Source numeric type.
     * @param other The source rectangle to convert from.
     * @return A new rectangle instance of type T.
     */
    template <typename U>
    static constexpr RectT from(const RectT<U>& other) noexcept {
        return RectT(
            static_cast<T>(other.left),
            static_cast<T>(other.top),
            static_cast<T>(other.right),
            static_cast<T>(other.bottom)
        );
    }

    /**
     * Converts the current rectangle to a specified numeric type.
     * @note Converting from floating-point to integral types results in truncation. Use ceiled() for pixel-enclosing
     * boundaries.
     * @tparam To Target numeric type.
     * @return A new rectangle instance of type To.
     */
    template <typename To>
    constexpr RectT<To> as() const noexcept {
        return *this;
    }

    /**
     * Returns the width of the rectangle.
     * @return The difference between right and left.
     */
    constexpr T width() const noexcept { return right - left; }

    /**
     * Sets a new width by adjusting the right boundary.
     * @param w New width value.
     * @return Reference to the current rectangle.
     */
    constexpr RectT& width(T w) noexcept {
        right = left + w;
        return *this;
    }

    /**
     * Returns the height of the rectangle.
     * @return The difference between bottom and top.
     */
    constexpr T height() const noexcept { return bottom - top; }

    /**
     * Sets a new height by adjusting the bottom boundary.
     * @param h New height value.
     * @return Reference to the current rectangle.
     */
    constexpr RectT& height(T h) noexcept {
        bottom = top + h;
        return *this;
    }

    /**
     * Checks if the rectangle has no visible area.
     *
     * A rectangle is considered empty if its width or height is less than or
     * equal to zero. This handles both collapsed rectangles and invalid
     * rectangles with negative dimensions.
     * @return True if the rectangle is empty or invalid.
     */
    constexpr bool isEmpty() const noexcept { return width() <= static_cast<T>(0) || height() <= static_cast<T>(0); }

    /**
     * Checks if the rectangle coordinates are logically consistent.
     * @return True if left <= right and top <= bottom.
     */
    constexpr bool isValid() const noexcept { return left <= right && top <= bottom; }

    /**
     * @name Corner Accessors
     * @{
     */
    /**
     * Returns the top-left corner point.
     * @return A PointT representing (left, top).
     */
    constexpr PointT<T> lt() const noexcept { return {left, top}; } // Left-Top

    /**
     * Returns the top-right corner point.
     * @return A PointT representing (right, top).
     */
    constexpr PointT<T> rt() const noexcept { return {right, top}; }

    /**
     * Returns the bottom-left corner point.
     * @return A PointT representing (left, bottom).
     */
    constexpr PointT<T> lb() const noexcept { return {left, bottom}; }

    /**
     * Returns the bottom-right corner point.
     * @return A PointT representing (right, bottom).
     */
    constexpr PointT<T> rb() const noexcept { return {right, bottom}; }

    ///@}

    /**
     * Calculates the center point of the rectangle.
     *
     * @note Returns a PointT<float> for accuracy unless T is already double/long double.
     * @warning If T is an integer type, the result will be truncated to the nearest integer
     * coordinate, potentially losing 0.5 units of precision.
     * @return The center point of the rectangle in its original coordinate type T.
     */
    constexpr PointT<T> center() const {
        if constexpr (std::is_floating_point_v<T>) {
            return {left + width() * static_cast<T>(0.5), top + height() * static_cast<T>(0.5)};
        } else {
            return {left + width() / 2, top + height() / 2};
        }
    }

    /**
     * Returns the dimensions of the rectangle.
     * @return A SizeT instance containing width and height.
     */
    constexpr SizeT<T> size() const { return Size(width(), height()); }

    /**
     * Resets all boundaries to zero.
     * @return Reference to the current rectangle.
     */
    RectT& clear() noexcept {
        left = top = right = bottom = static_cast<T>(0);
        return *this;
    }

    /**
     * Returns an integer rectangle aligned to the pixel grid that fully encloses this rectangle.
     * @note The method ensures no content is clipped by flooring the top-left corner and
     * ceiling the bottom-right corner. It is equivalent to a "pixel-enclosing" operation.
     * @return A new RectT<int> instance representing the aligned boundary.
     */
    RectT<int> aligned() const noexcept {
        if constexpr (std::is_integral_v<T>) {
            return as<int>();
        } else {
            return {
                math::floor_cast<int>(left),
                math::floor_cast<int>(top),
                math::ceil_cast<int>(right),
                math::ceil_cast<int>(bottom)
            };
        }
    }

    /**
     * Compares two rectangles for approximate equality.
     *
     * This method compares all four edges using the provided epsilon threshold.
     * @param other The rectangle to compare with.
     * @param epsilon The tolerance threshold.
     * @return True if all edges are approximately equal.
     */
    constexpr bool equals(const RectT& other, T epsilon = math::default_eps_v<T>) const noexcept {
        if constexpr (std::is_floating_point_v<T>) {
            return math::fuzzyEqual(left, other.left, epsilon) && math::fuzzyEqual(top, other.top, epsilon)
                   && math::fuzzyEqual(right, other.right, epsilon) && math::fuzzyEqual(bottom, other.bottom, epsilon);
        } else {
            BIX_UNUSED(epsilon)
            return *this == other;
        }
    }

    /**
     * @name Arithmetic Operators
     * @{
     */
    /**
     * Strict equality operator.
     *
     * Performs a zero-tolerance comparison on all edges using math::exactlyEqual.
     */
    constexpr bool operator==(const RectT& rhs) const noexcept {
        return math::exactlyEqual(left, rhs.left) && math::exactlyEqual(top, rhs.top)
               && math::exactlyEqual(right, rhs.right) && math::exactlyEqual(bottom, rhs.bottom);
    }

    ///@}
};
} // namespace geom

/** Type alias for a single-precision floating-point rectangle. */
using RectF = geom::RectT<float>;

/** Type alias for an integer rectangle. */
using RectI = geom::RectT<int>;

/** Default rectangle type alias using RectF. */
using Rect = RectF;
} // namespace bix