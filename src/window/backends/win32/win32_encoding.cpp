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

#include "window/backends/win32/win32_encoding.h"

#include <windows.h>

namespace bix::win32::encoding {
namespace internal {
/** @brief Threshold for stack allocation (in characters). 256 chars = 512 bytes. */
constexpr int kMaxStackChars = 256;

void convert_to_utf8_run(std::wstring_view wide, const std::function<void(const char*)>& callback) {
    const int input_len = static_cast<int>(wide.size());

    // 1. Calculate required size
    const int size_needed = WideCharToMultiByte(CP_UTF8, 0, wide.data(), input_len, nullptr, 0, nullptr, nullptr);

    if (size_needed <= 0) {
        callback("");
        return;
    }

    // 2. Stack vs Heap strategy
    if (size_needed < kMaxStackChars) {
        char buffer[kMaxStackChars + 1];
        if (WideCharToMultiByte(CP_UTF8, 0, wide.data(), input_len, buffer, size_needed, nullptr, nullptr) > 0) {
            buffer[size_needed] = '\0';
            callback(buffer);
            return;
        }
    } else {
        std::string heap_buf(size_needed, '\0');
        if (WideCharToMultiByte(CP_UTF8, 0, wide.data(), input_len, heap_buf.data(), size_needed, nullptr, nullptr)
            > 0) {
            callback(heap_buf.c_str());
            return;
        }
    }
    callback("");
}

void convert_to_wide_run(std::string_view utf8, const std::function<void(const wchar_t*)>& callback) {
    const int input_len = static_cast<int>(utf8.size());
    // Calculate required buffer size
    const int size_needed = MultiByteToWideChar(CP_UTF8, 0, utf8.data(), input_len, nullptr, 0);
    if (size_needed <= 0) {
        callback(L"");
        return;
    }

    // Optimization: Use stack if the string is short enough
    if (size_needed < kMaxStackChars) {
        wchar_t buffer[kMaxStackChars + 1];
        if (MultiByteToWideChar(CP_UTF8, 0, utf8.data(), input_len, buffer, size_needed) > 0) {
            buffer[size_needed] = L'\0';
            callback(buffer);
            return;
        }
    } else {
        // Fallback: Use heap for very long strings
        std::wstring heap_buf(size_needed, L'\0');
        if (MultiByteToWideChar(CP_UTF8, 0, utf8.data(), input_len, heap_buf.data(), size_needed) > 0) {
            callback(heap_buf.c_str());
            return;
        }
    }
    callback(L"");
}
} // namespace internal

std::wstring to_wstring(std::string_view utf8) {
    if (utf8.empty()) {
        return {};
    }
    const int input_len = static_cast<int>(utf8.size());
    // Calculate required buffer size
    const int size_needed = MultiByteToWideChar(CP_UTF8, 0, utf8.data(), input_len, nullptr, 0);
    if (size_needed <= 0) {
        return {};
    }
    std::wstring result(size_needed, L'\0');
    if (MultiByteToWideChar(CP_UTF8, 0, utf8.data(), input_len, result.data(), size_needed) <= 0) {
        return {};
    }
    return result;
}

std::string to_utf8(std::wstring_view wide) {
    if (wide.empty()) {
        return {};
    }
    const int input_len = static_cast<int>(wide.size());
    const int size_needed = WideCharToMultiByte(CP_UTF8, 0, wide.data(), input_len, nullptr, 0, nullptr, nullptr);
    if (size_needed <= 0) {
        return {};
    }
    std::string result(size_needed, '\0');
    if (WideCharToMultiByte(CP_UTF8, 0, wide.data(), input_len, result.data(), size_needed, nullptr, nullptr) <= 0) {
        return {};
    }
    return result;
}
} // namespace bix::win32::encoding