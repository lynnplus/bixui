/*
 * Copyright (c) 2025 Lynn <lynnplus90@gmail.com>.
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
#include "bixlib/common.h"
#include "bixlib/export_macro.h"
#include "bixlib/utils/numeric.h"

#include <string>

namespace bix {
class BIX_PUBLIC Color {
public:
    enum Spec {
        Invalid,
        RGB,
        HSV,
        HSL,
    };

    constexpr Color() = default;

    constexpr Color(int r, int g, int b, int a = 255) {
        if (!isRgbaValid(r, g, b, a)) {
            return;
        }
        mSpec = RGB;
        mData[0] = numeric_cast<uint8_t>(r);
        mData[1] = numeric_cast<uint8_t>(g);
        mData[2] = numeric_cast<uint8_t>(b);
        mData[3] = numeric_cast<uint8_t>(a);
    }

    /**
     * Convert hexadecimal color string to Color
     * @param str strings with format #rrggbbaa or #rrggbb, support case mixing
     * @return Color for bixlib
     */
    static Color fromHexString(const std::string& str);

    static constexpr bool isRgbaValid(int r, int g, int b, int a = 255) noexcept {
        if (r < 0 || g < 0 || b < 0 || a < 0)
            return false;
        return r <= 255 && g <= 255 && b <= 255 && a <= 255;
    }

    constexpr bool isValid() const noexcept { return mSpec != Invalid; }

    int red() const noexcept { return mData[0]; }

    int green() const noexcept { return mData[1]; }

    int blue() const noexcept { return mData[2]; }

    int alpha() const noexcept { return mData[3]; }

    bool operator==(const Color& rhs) const noexcept {
        return mSpec == rhs.mSpec && red() == rhs.red() && green() == rhs.green() && blue() == rhs.blue() &&
               alpha() == rhs.alpha();
    }

protected:
    Spec mSpec = Invalid;
    // r g b a pad
    uint8_t mData[5]{0, 0, 0, 0, 0};
};

namespace colors {
constexpr inline Color White{255, 255, 255};
constexpr inline Color Black{0, 0, 0};
constexpr inline Color Red{255, 0, 0};
constexpr inline Color Green{0, 255, 0};
constexpr inline Color Blue{0, 0, 255};
} // namespace colors
} // namespace bix
