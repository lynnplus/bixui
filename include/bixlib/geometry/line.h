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
 * @file line.h
 * 2D line segment geometry type definitions.
 */

#pragma once

#include <bixlib/geometry/point.h>
#include <bixlib/utils/numeric.h>

namespace bix::geom {

/**
 * @brief A template structure representing a finite line segment between two points.
 * @details In the context of this UI framework, 'Line' refers to a geometric line segment
 * with a defined start and end, consistent with industry-standard graphics APIs.
 * @tparam T The numeric type for coordinates, must satisfy the Real concept.
 */
template <Real T>
class BIX_PUBLIC LineT {
public:
    /** The starting point of the segment, initialized to (0,0) by default. */
    PointT<T> start{};

    /** The ending point of the segment, initialized to (0,0) by default. */
    PointT<T> end{};

    /** Default constructor initializing both points to zero. */
    constexpr LineT() = default;

    /**
     * Constructs a line from two points.
     *
     * @param s The starting point.
     * @param e The ending point.
     */
    constexpr LineT(const PointT<T>& s, const PointT<T>& e) noexcept : start(s), end(e) {}

    /**
     * Constructs a line from individual coordinates.
     *
     * @param x1 The x-coordinate of the start point.
     * @param y1 The y-coordinate of the start point.
     * @param x2 The x-coordinate of the end point.
     * @param y2 The y-coordinate of the end point.
     */
    constexpr LineT(T x1, T y1, T x2, T y2) noexcept : start(x1, y1), end(x2, y2) {}

    LineT(const LineT& other) = default;
    LineT& operator=(const LineT& other) = default;

    /**
     * Constructs a LineT from another compatible line segment type.
     *
     * Performs a type promotion of the line segment by converting both start
     * and end points. This is only available if the transition from From to T
     * is considered a safe upcast (e.g., LineI to LineF).
     *
     * @tparam From The source numeric type of the line segment.
     * @param other The source line segment to convert from.
     */
    template <typename From>
    requires Upcastable<T, From>
    constexpr explicit LineT(const LineT<From>& other) noexcept
        : start(PointT<T>(other.start))
        , end(PointT<T>(other.end)) {}

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
    constexpr LineT& operator=(const LineT<From>& other) noexcept {
        start = other.start;
        end = other.end;
        return *this;
    }

    /**
     * Explicitly creates a LineT from another numeric type.
     *
     * This static factory method is used to perform explicit type conversion.
     * When converting from floating-point to integral types, the values will be
     * **truncated** (fractional parts discarded).
     *
     * @tparam U Source numeric type (the type of \a other).
     * @param other The source line segment to convert from.
     * @return A new LineT instance in the current precision \a T.
     */
    template <typename U>
    static constexpr LineT from(const LineT<U>& other) noexcept {
        return {PointT<T>::from(other.start), PointT<T>::from(other.end)};
    }

    /** * Returns the horizontal displacement (delta X).
     * @return The value of end.x - start.x.
     */
    constexpr T dx() const noexcept { return end.x - start.x; }

    /** * Returns the vertical displacement (delta Y).
     * @return The value of end.y - start.y.
     */
    constexpr T dy() const noexcept { return end.y - start.y; }

    /** * Calculates the Euclidean length of the line segment.
     * @return The distance between start and end points.
     */
    auto length() const noexcept {
        const T d_x = dx();
        const T d_y = dy();
        return std::sqrt(d_x * d_x + d_y * d_y);
    }

    /**
     * Conveniently converts the current line to another numeric type.
     * A wrapper around LineT<To>::from(*this). Use this for chaining calls.
     *
     * @warning Note that precision **truncation** may occur depending on the target type.
     * @tparam To Target numeric type (e.g., float, int).
     * @return A LineT converted to the requested precision.
     */
    template <typename To>
    constexpr LineT<To> as() const noexcept {
        return LineT<To>::from(*this);
    }

    /**
     * Compares two line segments for approximate equality.
     *
     * Both start and end points must satisfy the fuzzy equality condition
     * based on the provided epsilon.
     * @param other The line to compare with.
     * @param epsilon The tolerance threshold.
     * @return True if the lines are approximately equal.
     */
    constexpr bool equals(const LineT& other, T epsilon = math::default_eps_v<T>) const noexcept {
        return start.equals(other.start, epsilon) && end.equals(other.end, epsilon);
    }

    /**
     * @name Arithmetic Operators
     * @{
     */
    /**
     * Checks if two line segments are identical.
     *
     * Performs a strict physical equality check on both endpoints using
     * math::exactlyEqual.
     */
    constexpr bool operator==(const LineT& other) const noexcept { return start == other.start && end == other.end; }

    ///@}
};

/** Alias for a single-precision floating-point line segment. @relatesalso LineT */
using LineF = LineT<float>;
/** Alias for an integer line segment. @relatesalso LineT */
using LineI = LineT<int>;
/** Default line segment type using float precision. @relatesalso LineT */
using Line = LineF;

} // namespace bix::geom
