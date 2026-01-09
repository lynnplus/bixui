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

#include <bixlib/utils/fmt_wrapper.h>

#include <string>
#include <string_view>
#include <utility>

#if defined(_MSC_VER)
    #define BIX_BREAK() __debugbreak()
#elif defined(__GNUC__) || defined(__clang__)
    #define BIX_BREAK() __builtin_trap()
#else
    #define BIX_BREAK() ((void)0)
#endif

namespace bix::internal {
void logAssertMessage(const char* file, int line, std::string_view message);

inline void handleAssertFailure(const char* f, int l, std::string_view msg) { logAssertMessage(f, l, msg); }

#ifdef BIX_HAS_LIBRARY
template <typename... Args>
void handleAssertFailure(const char* f, int l, fmt::format_string<Args...> fmt, Args&&... args) {
    logAssertMessage(f, l, fmt::format(fmt, std::forward<Args>(args)...));
}
#else
template <typename... Args>
void handleAssertFailure(const char* f, int l, std::string_view msg, Args&&...) {
    logAssertMessage(f, l, std::string(msg) + " [Warning: fmt missing]");
}
#endif

} // namespace bix::internal

#ifdef NDEBUG
    #define BIX_ASSERT(condition, ...) ((void)0)
#else
    #define BIX_ASSERT(condition, ...)                                             \
        do {                                                                       \
            if (!(condition)) {                                                    \
                bix::detail::handleAssertFailure(__FILE__, __LINE__, __VA_ARGS__); \
                BIX_BREAK();                                                       \
            }                                                                      \
        } while (false)
#endif