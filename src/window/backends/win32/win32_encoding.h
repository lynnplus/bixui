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

#include <functional>
#include <string>

namespace bix::win32::encoding {

namespace internal {
void convert_to_utf8_run(std::wstring_view wide, const std::function<void(const char*)>& callback);
void convert_to_wide_run(std::string_view utf8, const std::function<void(const wchar_t*)>& callback);
} // namespace internal

/**
 * Converts a UTF-8 string to a std::wstring.
 *
 * This function performs a heap allocation and returns a persistent object. * Ideal for storing text in class members
 * or long-lived variables.
 *
 * @param utf8 The input UTF-8 string view.
 * @return A std::wstring containing the UTF-16 representation.
 * @code
 * std::wstring myTitle = bix::win32::encoding::to_wstring("Hello World");
 * @endcode
 */
std::wstring to_wstring(std::string_view utf8);

/**
 * Converts a wide-character string (UTF-16) to a UTF-8 std::string.
 *
 * Used when receiving strings from Win32 APIs (e.g., GetWindowText) to
 * bring them back into the framework's UTF-8 ecosystem.
 * @param wide The input UTF-16 string view.
 * @return A std::string containing the UTF-8 representation.
 */
std::string to_utf8(std::wstring_view wide);

/**
 * High-performance scoped conversion from UTF-8 to a wide-character pointer.
 * * This function uses stack allocation (SBO/alloca) for short strings to avoid heap overhead.
 * It is specifically designed for transient Win32 API calls.

 * @tparam F A callable object (Lambda, std::function, etc.)
 * @param utf8 The input UTF-8 string view.
 * @param func A callback that accepts the temporary `const wchar_t*`.
 * @note For strings shorter than 256 characters, memory is allocated on the stack.
 * @warning The `const wchar_t*` passed to the lambda is only valid **inside** the scope
 * of the lambda. Do NOT store or return this pointer for later use.
 * @code
 * bix::win32::encoding::with_wide_ptr("Window Title", [&](const wchar_t* ptr) {
 * ::SetWindowTextW(hwnd, ptr); // Safe: used immediately
 * });
 * @endcode
 */
template <typename F>
void with_wide_ptr(std::string_view utf8, F&& func) {
    if (utf8.empty()) {
        func(L"");
        return;
    }
    internal::convert_to_wide_run(utf8, std::forward<F>(func));
}

/**
 * @brief Scoped conversion from UTF-16 to a temporary UTF-8 pointer.
 * @note Efficiently handles short strings via stack allocation.
 * @warning The `const char*` is only valid inside the callback.
 */
template <typename F>
void with_utf8_ptr(std::wstring_view wide, F&& func) {
    if (wide.empty()) {
        func("");
        return;
    }
    internal::convert_to_utf8_run(wide, std::forward<F>(func));
}

} // namespace bix::win32::encoding
