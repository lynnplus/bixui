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
#include <string>

#include "bixlib/common.h"
#include "bixlib/export_macro.h"
#include "bixlib/utils/numeric.h"

namespace bix {
class BIX_PUBLIC Color {
public:
    enum Spec { Invalid, Rgba, };

    constexpr Color() = default;

    constexpr Color(int r, int g, int b, int a = 255) {
        if (!isRgbaValid(r, g, b, a)) {
            return;
        }
        mSpec = Rgba;
        data[0] = numeric_cast<uint8_t>(r);
        data[1] = numeric_cast<uint8_t>(g);
        data[2] = numeric_cast<uint8_t>(b);
        data[3] = numeric_cast<uint8_t>(a);
    }

    /**
     * Convert hexadecimal color string to Color
     * @param str strings with format #rrggbbaa or #rrggbb, support case mixing
     * @return Color for bixlib
     */
    static Color fromHexString(const std::string& str);

    static constexpr bool isRgbaValid(int r, int g, int b, int a = 255) noexcept {
        if (r < 0 || g < 0 || b < 0 || a < 0) return false;
        return r <= 255 && g <= 255 && b <= 255 && a <= 255;
    }

    [[nodiscard]]
    constexpr bool isValid() const noexcept {
        return mSpec != Invalid;
    }

    [[nodiscard]]
    int red() const noexcept {
        return data[0];
    }

    [[nodiscard]]
    int green() const noexcept {
        return data[1];
    }

    [[nodiscard]]
    int blue() const noexcept {
        return data[2];
    }

    [[nodiscard]]
    int alpha() const noexcept {
        return data[3];
    }

protected:
    Spec mSpec = Invalid;
    //r g b a pad
    uint8_t data[5]{0, 0, 0, 0, 0};
};

namespace colors {
    constexpr inline Color White{255, 255, 255};
    constexpr inline Color Black{0, 0, 0};
    constexpr inline Color Red{255, 0, 0};
    constexpr inline Color Green{0, 255, 0};
    constexpr inline Color Blue{0, 0, 255};
}
}
