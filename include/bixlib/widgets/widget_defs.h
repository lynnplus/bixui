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
#include <bixlib/utils/flags.h>

#include <cstdint>

namespace bix {
enum class Visibility : uint8_t {
    Visible,
    Invisible,
    Collapsed
};

/**
 * @enum WidgetFlag
 * Internal state and dirty markers for a Widget.
 */
enum class WidgetFlag : uint32_t {
    Disable = 1 << 0,     ///< Non-interactive state.
    Clickable = 1 << 1,   ///< Responds to mouse clicks.
    DirtyPaint = 1 << 2,  ///< Needs repainting.
    DirtyLayout = 1 << 3, ///< Needs remeasuring and relayout.
    BoundsClip = 1 << 4,  ///< Enable clipping to bounds.
    Focusable = 1 << 5,   ///<
    InLayout = 1 << 11,   ///<
    InMeasure = 1 << 12,  ///<
    WillNotDraw = 1 << 8, ///<
    Opaque = 1 << 9,      ///<
};

BIX_DECLARE_ENUM_FLAGS(WidgetFlag)
using WidgetFlags = bix::Flags<WidgetFlag>;
} // namespace bix
