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

#include <cmath>
#include <cstdint>
#include <string_view>

namespace bix {

/**
 * A unified, high-precision UI dimension container supporting multiple units and layout modes.
 *
 * This class handles multiple units (DP, PX, PERCENT, VW, VH) and special * layout modes (AUTO, STRETCH). It uses
 * fixed-point integer arithmetic (scaled by 100) to ensure bit-identical rendering across different CPU architectures
 * and operating systems.
 *
 * @warning Precision: Because we use a scale of 100, the maximum precision is two decimal
 * places (e.g., 12.345 becomes 12.35). For standard UI design specs, this is sufficient.
 */
class BIX_PUBLIC Length {
public:
    /// Multiplier used to store floating-point values as fixed-point integers.
    static constexpr float FLOAT_SCALE = 100.f;
    /// Sentinel value used to signal a "Stretch to parent" layout intent.
    static constexpr int VAL_STRETCH = -1;
    /// Sentinel value used to signal a "Wrap around content" layout intent.
    static constexpr int VAL_AUTO = -2;
    /// Sentinel value representing an unconstrained or infinite dimension.
    /// Often used as the default value for maximum width or height constraints.
    static constexpr int VAL_INFINITY = -3;

    /**
     * @enum Unit
     * @brief Identifiers for the measurement system used by the Length instance.
     */
    enum Unit : uint8_t {
        NONE,    ///< Uninitialized or invalid unit.
        DP,      ///< Density-independent Pixels (Fixed-point x100).
        PX,      ///< Absolute Pixels (Integer)
        PERCENT, ///< Percentage relative to a reference dimension (e.g., parent width for margin, self width for
                 ///< translate).
        VW,      ///< Relative to 1% of canvas width (Fixed-point x100).
        VH,      ///< Relative to 1% of canvas height (Fixed-point x100).
        SPECIAL  ///< Used for AUTO and STRETCH sentinel values.
    };

    /**
     * Default constructor creating an invalid Length object.
     *
     * The unit is initialized to NONE, and the fixed value to 0.
     * Use isValid() to check for this state before layout calculations.
     */
    constexpr Length() noexcept = default;

    /**
     * @name Factory Methods
     * These methods provide a fluent API for creating Length objects.
     * @{
     */
    constexpr static Length dp(float v) noexcept { return {v, DP}; }

    constexpr static Length px(int v) noexcept { return {v, PX}; }

    constexpr static Length percent(float px) noexcept { return {px, PERCENT}; }

    constexpr static Length vw(float v) noexcept { return {v, VW}; }

    constexpr static Length vh(float v) noexcept { return {v, VH}; }

    constexpr static Length stretch() noexcept { return {VAL_STRETCH, SPECIAL}; }

    constexpr static Length autoSize() noexcept { return {VAL_AUTO, SPECIAL}; }

    constexpr static Length infinity() noexcept { return {VAL_INFINITY, SPECIAL}; }

    ///@}

    /**
     * Parses a raw string view into a Length object.
     *
     * @param[in] str The string to parse (e.g., "12.5dp", "50%", "auto").
     * @return A valid Length object if parsing succeeds; otherwise, returns px(0).
     * @note If a numeric value is provided without a unit suffix, it is
     * treated as **DP** by default (e.g., "16" is equivalent to "16dp").
     * @note Keywords "auto" and "stretch" must be either **all-lowercase** * or **all-uppercase**
     * (e.g., "auto" and "AUTO" are valid, but "Auto" is not).
     * @section Parsing_Rules Parsing Test Cases
     * The following table shows the expected results for various input strings.
     * These rules are integrated into unit tests to ensure consistent cross-platform behavior.
     * | Input String | Expected Unit | Internal Value (FixedValue) | Note |
     * | :----------- | :------------ | :------------------------- | :--- |
     * | "16"          | @c DP         | @c 1600                    | **Default**: Omitted unit defaults to DP. |
     * | "12.5dp"      | @c DP         | @c 1250                    | Standard lowercase unit. |
     * | "100px"       | @c PX         | @c 100                     | Absolute pixels, no scaling applied. |
     * | "50.5%"       | @c PERCENT    | @c 5050                    | Fixed-point conversion (50.5 * 100). |
     * | "auto"        | @c SPECIAL    | @c VAL_AUTO (-2)           | Supported lowercase keyword. |
     * | "AUTO"        | @c SPECIAL    | @c VAL_AUTO (-2)           | Supported uppercase keyword. |
     * | "Auto"        | @c PX         | @c 0                       | **Failure**: Mixed case not supported, returns 0px. |
     * | "invalid"     | @c PX         | @c 0                       | **Failure**: Returns default pixel value. |
     */
    static Length parse(std::string_view str) noexcept;

    /**
     * Resolves the stored unit into absolute physical pixels.
     * @param[in] ctx The rendering context containing screen density and canvas size.
     * @param[in] relative The reference size for percentage calculations (e.g., parent width).
     * @return The final resolved integer pixel value.
     * @warning Special modes (AUTO/STRETCH) return their sentinel values (-2/-1).
     * The layout engine must check #isAuto() or #isStretch() before
     * using the returned pixel value for rendering.
     */
    // int resolve(const Context& ctx, int relative) {
    //     assert(isValid());
    //
    //     if (mUnit == SPECIAL || mUnit == PX) {
    //         return mFixedValue;
    //     }
    //
    //     float multiplier = 1.0f;
    //     switch (mUnit) {
    //     case DP:
    //         multiplier = ctx.display().density();
    //         break;
    //     case PERCENT:
    //         multiplier = static_cast<float>(relative);
    //         break;
    //     case VW:
    //         multiplier = static_cast<float>(ctx.canvasSize().width);
    //         break;
    //     case VH:
    //         multiplier = static_cast<float>(ctx.canvasSize().height);
    //         break;
    //     default:
    //         return mFixedValue;
    //     }
    //
    //     // Standard rounding to nearest pixel to avoid shimmering/gaps.
    //     return static_cast<int>(std::round((static_cast<float>(mFixedValue) * multiplier) / FLOAT_SCALE));
    // }

    /** Checks if the length is set to 'auto'. */
    bool isAuto() const { return mUnit == SPECIAL && mFixedValue == VAL_AUTO; }

    /** Checks if the length is set to 'stretch'. */
    bool isStretch() const { return mUnit == SPECIAL && mFixedValue == VAL_STRETCH; }

    /** Checks if the length represents an infinite value. */
    bool isInfinity() const noexcept { return mUnit == SPECIAL && mFixedValue == VAL_INFINITY; }

    /** Returns the unit type. */
    Unit unit() const noexcept { return mUnit; }

    /** Checks if the unit is valid (not NONE). */
    bool isValid() const noexcept { return mUnit != NONE; }

    /**
     * Compares two Length objects for equality.
     * @return True if both the unit and the underlying fixed-point value are identical.
     */
    constexpr bool operator==(const Length& rhs) noexcept {
        return mFixedValue == rhs.mFixedValue && mUnit == rhs.mUnit;
    }

private:
    int mFixedValue = 0;
    Unit mUnit = NONE;

    // ReSharper disable once CppDFAConstantParameter
    constexpr Length(int v, Unit unit) : mFixedValue(v), mUnit(unit) {}

    constexpr Length(float v, Unit unit) : mUnit(unit) { mFixedValue = static_cast<int>(std::round(v * FLOAT_SCALE)); }
};

/**
 * User-defined literals for expressive Length creation.
 *
 * These literals allow you to write dimensions naturally, such as @c 16_dp or @c 50_percent.
 * To use these literals, add the following directive to your code:
 * @code
 * using namespace bix::length_literals;
 * auto width = 100.5_dp;
 * @endcode
 */
namespace literals {

/**
 * Literal for creating a Length in Density-independent Pixels.
 * @code
 * auto padding = 16.5_dp;
 * @endcode
 */
inline Length operator""_dp(long double value) { return Length::dp(static_cast<float>(value)); }

/**
 * Literal for creating a Length as a percentage.
 * @code
 * auto width = 50.0_percent;
 * @endcode
 */
inline Length operator""_percent(long double value) { return Length::percent(static_cast<float>(value)); }

/**
 * Literal for creating a Length in Viewport Width units.
 * @code
 * auto fullWidth = 100.0_vw;
 * @endcode
 */
inline Length operator""_vw(long double value) { return Length::vw(static_cast<float>(value)); }

/**
 * Literal for creating a Length in Viewport Height units.
 * @code
 * auto fullHeight = 100.0_vh;
 * @endcode
 */
inline Length operator""_vh(long double value) { return Length::vh(static_cast<float>(value)); }

} // namespace length_literals

} // namespace bix
