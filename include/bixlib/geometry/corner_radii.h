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
 * @file corner_radii.h
 * @brief Corner radii definitions for rounded shapes.
 */

#pragma once

#include <bixlib/export_macro.h>
#include <bixlib/utils/concepts.h>

namespace bix {

namespace geom {
/**
 * Defines the radii for the four corners of a rectangle.
 *
 * CornerRadiiT specifies the curvature for each corner. It supports
 * independent values for each corner to enable complex UI shapes like
 * tab bars or grouped list items.
 *
 * @tparam T The numeric type for the radius.
 */
template <Real T>
struct BIX_PUBLIC CornerRadiiT {
    T topLeft{0};
    T topRight{0};
    T bottomLeft{0};
    T bottomRight{0};

    constexpr CornerRadiiT() noexcept = default;

    /**
     * Constructs CornerRadiiT with explicit values for each corner.
     */
    constexpr CornerRadiiT(T tl, T tr, T bl, T br) noexcept
        : topLeft(tl)
        , topRight(tr)
        , bottomLeft(bl)
        , bottomRight(br) {}

    /** Creates corner radii where all corners have the same value. */
    static constexpr CornerRadiiT all(T radius) noexcept { return {radius, radius, radius, radius}; }

    /** * Creates corner radii for only the top edge.
     * Bottom corners will be zero (sharp).
     */
    static constexpr CornerRadiiT top(T radius) noexcept { return {radius, radius, 0, 0}; }

    /** * Creates corner radii for only the bottom edge.
     * Top corners will be zero (sharp).
     */
    static constexpr CornerRadiiT bottom(T radius) noexcept { return {0, 0, radius, radius}; }

    /** * Creates corner radii for only the left edge.
     */
    static constexpr CornerRadiiT left(T radius) noexcept { return {radius, 0, radius, 0}; }

    /** * Creates corner radii for only the right edge.
     */
    static constexpr CornerRadiiT right(T radius) noexcept { return {0, radius, 0, radius}; }
};
} // namespace geom

using CornerRadiusF = geom::CornerRadiiT<float>;
using CornerRadiusI = geom::CornerRadiiT<int>;
using CornerRadius = CornerRadiusF;
} // namespace bix
