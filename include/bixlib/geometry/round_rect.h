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
 * @file round_rect.h
 * @brief Rounded rectangle (rectangle + corner radii) definitions.
 */

#pragma once

#include <bixlib/geometry/corner_radii.h>
#include <bixlib/geometry/rect.h>

namespace bix {
namespace geom {

/**
 * A rectangle with rounded corners.
 *
 * RoundRectT combines a bounding rectangle (RectT) with corner radii
 * (CornerRadiiT). This is the fundamental primitive for rendering
 * buttons, panels, and other rounded UI elements.
 *
 * @tparam T The numeric type for coordinates and dimensions.
 */
template <Real T>
struct BIX_PUBLIC RoundRectT {
    /** The bounding rectangle defining the outer limits. */
    RectT<T> rect;

    /** The radii for each of the four corners. */
    CornerRadiiT<T> radii;

    /** Default constructor initializing an empty rectangle with no curvature. */
    constexpr RoundRectT() noexcept = default;

    /**
     * Constructs a RoundRectT from a rectangle and a uniform radius.
     *
     * @param r The bounding rectangle.
     * @param radius The radius to apply to all four corners.
     */
    constexpr RoundRectT(const RectT<T>& r, T radius) noexcept : rect(r), radii(CornerRadiiT<T>::all(radius)) {}

    /**
     * Constructs a RoundRectT with explicit rectangle and radii values.
     *
     * @param r The bounding rectangle.
     * @param rad The corner radii object.
     */
    constexpr RoundRectT(const RectT<T>& r, const CornerRadiiT<T>& rad) noexcept : rect(r), radii(rad) {}

    /**
     * Conversion constructor from another compatible RoundRectT type.
     *
     * Utilizes Upcastable to allow safe conversion between types (e.g., int to float).
     * Marked as explicit to prevent accidental precision loss in layout logic.
     *
     * @tparam From The source numeric type.
     * @param other The source rounded rectangle to convert from.
     */
    template <typename From>
    requires Upcastable<T, From>
    constexpr explicit RoundRectT(const RoundRectT<From>& other) noexcept
        : rect(RectT<T>(other.rect))
        , radii(
              static_cast<T>(other.radii.topLeft), static_cast<T>(other.radii.topRight),
              static_cast<T>(other.radii.bottomLeft), static_cast<T>(other.radii.bottomRight)
          ) {}

    /**
     * Determines if this object is a simple rectangle.
     *
     * @return True if all corner radii are zero, indicating no curvature.
     */
    constexpr bool isRect() const noexcept {
        return radii.topLeft == 0 && radii.topRight == 0 && radii.bottomLeft == 0 && radii.bottomRight == 0;
    }
};

} // namespace geom

/**
 * Alias for RoundRectT using float.
 *
 * This is the standard version used for most UI layout and rendering
 * calculations to maintain sub-pixel precision.
 */
using RoundRectF = geom::RoundRectT<float>;

/**
 * Alias for RoundRectT using int.
 *
 * Primarily used for pixel-perfect snapping or scenarios where integer
 * coordinates are required to avoid anti-aliasing artifacts.
 */
using RoundRectI = geom::RoundRectT<int>;

/**
 * Default RoundRect alias.
 *
 * Points to RoundRectF by default, following the framework's convention
 * of using floating-point numbers as the primary numeric type.
 */
using RoundRect = RoundRectF;
} // namespace bix
