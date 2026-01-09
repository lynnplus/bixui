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

#include <bixlib/core/length.h>

#include <charconv>

namespace bix {

Length Length::parse(std::string_view str) noexcept {
    if (str.empty()) {
        return px(0);
    }
    if (str == "auto" || str == "AUTO") {
        return autoSize();
    }
    if (str == "stretch" || str == "STRETCH") {
        return stretch();
    }

    float val = 0.0f;
    // Fast, zero-allocation, locale-independent float parsing
    auto [ptr, ec] = std::from_chars(str.data(), str.data() + str.size(), val);

    if (ec != std::errc{})
        return px(0);

    std::string_view unitPart(ptr, (str.data() + str.size()) - ptr);
    if (unitPart.empty() || unitPart == "dp") {
        return dp(val);
    }
    if (unitPart == "px") {
        return px(static_cast<int>(val));
    }
    if (unitPart == "%") {
        return percent(val);
    }
    if (unitPart == "vw") {
        return vw(val);
    }
    if (unitPart == "vh") {
        return vh(val);
    }
    return px(0);
}
} // namespace bix