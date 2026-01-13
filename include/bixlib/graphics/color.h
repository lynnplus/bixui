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

#include <bixlib/export_macro.h>
#include <bixlib/utils/numeric.h>

#include <string>

namespace bix {

/**
 * Represents a color using various color spaces like RGB, HSV, and HSL.
 *
 * This class provides a unified way to handle colors within the graphics system,
 * supporting hexadecimal parsing and basic color space validation.
 */
class BIX_PUBLIC Color {
public:
    /**
     * Defines the color specification or color space used for storage.
     */
    enum class Spec : uint8_t {
        Invalid, ///< Indicates an uninitialized or invalid color state.
        RGB,     ///< Standard Red-Green-Blue color space.
        HSV,     ///< Hue-Saturation-Value color space.
        HSL,     ///< Hue-Saturation-Lightness color space.
    };

    /**
     * Default constructor creating an invalid color.
     */
    constexpr Color() = default;

    /**
     * Constructs a color from integer RGBA components.
     *
     * @param r Red component [0, 255].
     * @param g Green component [0, 255].
     * @param b Blue component [0, 255].
     * @param a Alpha component [0, 255].
     */
    constexpr Color(int r, int g, int b, int a = 255) {
        if (!isRgbaValid(r, g, b, a)) { return; }
        mSpec = Spec::RGB;
        mData[0] = math::numeric_cast<uint8_t>(r);
        mData[1] = math::numeric_cast<uint8_t>(g);
        mData[2] = math::numeric_cast<uint8_t>(b);
        mData[3] = math::numeric_cast<uint8_t>(a);
    }

    /**
     * Creates a Color object from a hexadecimal string.
     *
     * Supports the following formats:
     * - 3-digit: "#RGB" or "RGB"
     * - 6-digit: "#RRGGBB" or "RRGGBB"
     * - 8-digit: "#RRGGBBAA" or "RRGGBBAA"
     * @code
     * auto c1 = Color::fromHexString("#FF0000");      // Opaque Red
     * auto c2 = Color::fromHexString("00FF0080");    // Semi-transparent Green
     * auto c3 = Color::fromHexString("#F00");        // Shorthand Red (expands to `#FF0000`)
     * @endcode
     * @param str Hexadecimal string representation.
     * @return A parsed Color object, or an invalid Color if parsing fails.
     * @note This method is case-insensitive (e.g., "#ff0000" and "#FF0000" are equal).
     * @note The '#' prefix is optional.
     */
    static Color fromHexString(std::string_view str);

    /**
     * Creates a Color object from floating-point RGB components.
     *
     * @param r Red [0.0, 1.0].
     * @param g Green [0.0, 1.0].
     * @param b Blue [0.0, 1.0].
     * @param a Alpha [0.0, 1.0].
     * @return A Color object with mapped uint8 values.
     */
    static constexpr Color fromRgbF(float r, float g, float b, float a = 1.0f) {
        return {
            math::round_cast<uint8_t>(r * 255.0f),
            math::round_cast<uint8_t>(g * 255.0f),
            math::round_cast<uint8_t>(b * 255.0f),
            math::round_cast<uint8_t>(a * 255.0f)
        };
    }

    /**
     * Validates if the given integer RGBA components are within range.
     *
     * @return True if all components are in [0, 255], false otherwise.
     */
    static constexpr bool isRgbaValid(int r, int g, int b, int a = 255) noexcept {
        if (r < 0 || g < 0 || b < 0 || a < 0) return false;
        return r <= 255 && g <= 255 && b <= 255 && a <= 255;
    }

    /**
     * Checks if the color has a valid specification.
     *
     * @return True if mSpec is not Spec::Invalid.
     */
    constexpr bool isValid() const noexcept { return mSpec != Spec::Invalid; }

    /**
     * Linearly interpolates between two colors.
     *
     * @param a The start color.
     * @param b The end color.
     * @param t The interpolation factor [0.0, 1.0].
     * @return The resulting blended color.
     */
    static Color lerp(const Color& a, const Color& b, float t) noexcept;

    /**
     * Returns the red component value.
     *
     * @return Integer value in range [0, 255].
     */
    int red() const noexcept { return mData[0]; }

    /**
     * Sets the red component and returns a reference.
     *
     * @param r New red value [0, 255].
     * @return Reference to this object for chaining.
     */
    Color& red(int r) {
        mData[0] = math::numeric_cast<uint8_t>(r);
        return *this;
    }

    /**
     * Returns the green component value.
     *
     * @return Integer value in range [0, 255].
     */
    int green() const noexcept { return mData[1]; }

    /**
     * Sets the green component and returns a reference.
     *
     * @param g New green value [0, 255].
     * @return Reference to this object for chaining.
     */
    Color& green(int g) {
        mData[1] = math::numeric_cast<uint8_t>(g);
        return *this;
    }

    /**
     * Returns the blue component value.
     *
     * @return Integer value in range [0, 255].
     */
    int blue() const noexcept { return mData[2]; }

    /**
     * Sets the blue component and returns a reference.
     *
     * @param b New blue value [0, 255].
     * @return Reference to this object for chaining.
     */
    Color& blue(int b) {
        mData[2] = math::numeric_cast<uint8_t>(b);
        return *this;
    }

    /**
     * Returns the alpha component value.
     *
     * @return Integer value in range [0, 255].
     */
    int alpha() const noexcept { return mData[3]; }

    /**
     * Sets the alpha component and returns a reference.
     *
     * @param a New alpha value [0, 255].
     * @return Reference to this object for chaining.
     */
    Color& alpha(int a) {
        mData[3] = math::numeric_cast<uint8_t>(a);
        return *this;
    }

    /**
     * Sets the alpha component using a floating-point value.
     *
     * @param a Normalized alpha [0.0, 1.0].
     * @return Reference to this object for chaining.
     */
    Color& alphaF(float a) {
        mData[3] = math::round_cast<uint8_t>(a * 255.0f);
        return *this;
    }

    /**
     * Returns the alpha component as a floating-point value.
     *
     * @return Normalized alpha in range [0.0, 1.0].
     */
    float alphaF() const noexcept { return static_cast<float>(alpha()) / 255.0f; }

    /**
     * Returns a copy of the color with a modified alpha value.
     *
     * @param a Normalized alpha [0.0, 1.0].
     * @return A new Color object.
     */
    Color withAlphaF(float a) const { return {red(), green(), blue(), math::round_cast<int>(a * 255.0f)}; }

    /**
     * Compares two colors for equality.
     *
     * @return True if both colors have the same spec and components.
     */
    bool operator==(const Color& rhs) const noexcept {
        return mSpec == rhs.mSpec && red() == rhs.red() && green() == rhs.green() && blue() == rhs.blue()
               && alpha() == rhs.alpha();
    }

protected:
    Spec mSpec = Spec::Invalid;
    // r g b a
    uint8_t mData[4]{0, 0, 0, 0};
};
} // namespace bix
