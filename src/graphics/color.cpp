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

#include "bixlib/graphics/color.h"

#include <algorithm>

using namespace std;

namespace bix {

namespace {
constexpr int hexCharToInt(char c) noexcept {
    if (c >= '0' && c <= '9') return c - '0';
    if (c >= 'a' && c <= 'f') return c - 'a' + 10;
    if (c >= 'A' && c <= 'F') return c - 'A' + 10;
    return -1;
}
} // namespace

Color Color::fromHexString(string_view str) {
    if (!str.empty() && str[0] == '#') { str.remove_prefix(1); }

    size_t len = str.length();
    if (len != 3 && len != 6 && len != 8) { return {}; }

    auto parsePair = [&](size_t pos) -> int {
        int h1 = hexCharToInt(str[pos]);
        int h2 = hexCharToInt(str[pos + 1]);
        return (h1 < 0 || h2 < 0) ? -1 : (h1 << 4) | h2;
    };

    auto parseSingle = [&](size_t pos) -> int {
        int h = hexCharToInt(str[pos]);
        return (h < 0) ? -1 : (h << 4) | h;
    };

    const int r = len == 3 ? parseSingle(0) : parsePair(0);
    const int g = len == 3 ? parseSingle(1) : parsePair(2);
    const int b = len == 3 ? parseSingle(2) : parsePair(4);
    const int a = len == 8 ? parsePair(6) : 255;

    if (r < 0 || g < 0 || b < 0 || a < 0) { return {}; }

    return {r, g, b, a};
}

Color Color::lerp(const Color& a, const Color& b, float t) noexcept {
    t = std::clamp(t, 0.0f, 1.0f);

    auto lerpChannel = [t](int start, int end) -> int {
        auto f_start = static_cast<float>(start);
        auto f_end = static_cast<float>(end);
        return math::round_cast<int>(f_start + (f_end - f_start) * t);
    };

    return {
        lerpChannel(a.red(), b.red()),
        lerpChannel(a.green(), b.green()),
        lerpChannel(a.blue(), b.blue()),
        lerpChannel(a.alpha(), b.alpha())
    };
}

} // namespace bix
