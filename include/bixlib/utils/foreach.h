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

namespace bix::utils {

template <std::ranges::range Range, typename Function>
requires std::is_invocable_r_v<bool, Function, std::ranges::range_value_t<Range>&>
void forRange(Range& iterable, const Function& func) {
    for (auto it = iterable.begin(); it != iterable.end(); ++it) {
        if (!func(*it)) {
            return;
        }
    }
}

template <std::ranges::range Range, typename Function>
requires std::is_invocable_r_v<bool, Function, std::ranges::range_value_t<Range>&>
bool anyOf(Range& iterable, const Function& func) {
    for (auto it = iterable.begin(); it != iterable.end(); ++it) {
        if (func(*it)) {
            return true;
        }
    }
    return false;
}

} // namespace bix::utils