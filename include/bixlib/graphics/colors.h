/*
 * Copyright (c) 2026 Lynn <lynnplus90@gmail.com>
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

#include <bixlib/graphics/color.h>

namespace bix {

namespace literals {

/**
 * Literal operator for creating a Color from a hexadecimal string.
 *
 * Supports formats like `#RRGGBB` or `#RRGGBBAA`.
 * @code
 * using namespace bix::literals;
 * auto c = "#FF0000"_rgba;
 * @endcode
 *
 * @param hex The hexadecimal string literal.
 * @param len The length of the string.
 * @return A Color object parsed from the hex string.
 */
inline Color operator""_rgba(const char* hex, std::size_t len) {
    return Color::fromHexString(std::string_view(hex, len));
}

/**
 * Creates a Color from a 24-bit hex integer.
 * @note This operator treats the integer as a standard 24-bit value (0xRRGGBB).
 * If a shorter value is provided (e.g., `0xFF_rgb`), it is treated as `0x0000FF`.
 * It does NOT follow CSS-style short-hex rules (like `#F00` -> `#FF0000`).
 * @param val The color value in 0xRRGGBB format.
 * @return A Color object with alpha set to 255.
 */
inline Color operator""_rgb(unsigned long long val) {
    return {
        static_cast<uint8_t>((val >> 16) & 0xFF),
        static_cast<uint8_t>((val >> 8) & 0xFF),
        static_cast<uint8_t>(val & 0xFF)
    };
}

/**
 * @brief Creates a Color from a 32-bit hex integer (0xRRGGBBAA).
 * @warning Be careful with short values. For example, `0xFF_rgba` is interpreted
 * as `0x000000FF` (black with full alpha), not a short version of red.
 * Always provide the full 8-digit hex for clarity.
 * @code
 * auto semi_red = 0xFF000080_rgba; // Red with 50% alpha
 * @endcode
 * @param val The 32-bit color value.
 * @return A Color object.
 */
inline Color operator""_rgba(unsigned long long val) {
    return {
        static_cast<uint8_t>((val >> 24) & 0xFF),
        static_cast<uint8_t>((val >> 16) & 0xFF),
        static_cast<uint8_t>((val >> 8) & 0xFF),
        static_cast<uint8_t>(val & 0xFF)
    };
}
} // namespace literals

/**
 * Standard color constants for the bixlib graphics system.
 *
 * This file provides a collection of predefined Color objects in the bix::colors
 * namespace, allowing for easy access to common colors.
 */
namespace colors {
/** Fully transparent color with zero alpha. */
constexpr inline Color Transparent{0, 0, 0, 0};
/** Opaque white color (`#FFFFFF`). */
constexpr inline Color White{255, 255, 255};
/** Opaque black color (`#000000`). */
constexpr inline Color Black{0, 0, 0};
/** Pure red color (`#FF0000`). */
constexpr inline Color Red{255, 0, 0};
/** Pure green color (`#00FF00`). */
constexpr inline Color Green{0, 255, 0};
/** Pure blue color (`#0000FF`). */
constexpr inline Color Blue{0, 0, 255};
/** Middle gray color (`#808080`). */
constexpr inline Color Gray{128, 128, 128};
/** Bright yellow color (`#FFFF00`). */
constexpr inline Color Yellow{255, 255, 0};
/** Cyan color (`#00FFFF`). */
constexpr inline Color Cyan{0, 255, 255};
/** Magenta color (`#FF00FF`). */
constexpr inline Color Magenta{255, 0, 255};
} // namespace colors
} // namespace bix