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
 * @file insets.h
 * @brief Edge insets (left, top, right, bottom) definitions for layout.
 */

#pragma once
#include <bixlib/core/length.h>

namespace bix {

/**
 * A structure representing the offsets for the four edges of a rectangle.
 *
 * EdgeInsets is used to define margins or padding in the layout system.
 * Unlike geometric primitives that use raw numeric types, this uses Length
 * to support various units like pixels, percentages, or relative units.
 */
struct EdgeInsets {
    /** The offset from the left edge. */
    Length left{Length::px(0)};

    /** The offset from the top edge. */
    Length top{Length::px(0)};

    /** The offset from the right edge. */
    Length right{Length::px(0)};

    /** The offset from the bottom edge. */
    Length bottom{Length::px(0)};

    /** Default constructor initializing all edges to zero pixels. */
    constexpr EdgeInsets() noexcept = default;

    /**
     * Constructs EdgeInsets with explicit values for each edge.
     *
     * @param l The left offset.
     * @param t The top offset.
     * @param r The right offset.
     * @param b The bottom offset.
     */
    constexpr EdgeInsets(Length l, Length t, Length r, Length b) noexcept : left(l), top(t), right(r), bottom(b) {}

    /**
     * Creates EdgeInsets where all four edges have the same offset.
     *
     * @param v The length to apply to all edges.
     * @return A new EdgeInsets instance.
     */
    static constexpr EdgeInsets all(Length v) noexcept { return EdgeInsets(v, v, v, v); }

    /**
     * Creates EdgeInsets with symmetrical vertical and horizontal offsets.
     *
     * @param vertical The offset to apply to top and bottom.
     * @param horizontal The offset to apply to left and right.
     * @return A new EdgeInsets instance.
     */
    static constexpr EdgeInsets symmetric(Length vertical, Length horizontal) noexcept {
        return EdgeInsets(horizontal, vertical, horizontal, vertical);
    }
};

} // namespace bix
