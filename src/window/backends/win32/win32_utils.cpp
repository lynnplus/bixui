
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

#include "window/backends/win32/win32_utils.h"

#include "window/backends/win32/win32_encoding.h"

namespace bix::win32::utils {

std::string format_win32_error(DWORD errorCode) {
    if (errorCode == ERROR_SUCCESS) {
        return "Success (0x0000)";
    }

    LPWSTR buffer = nullptr;
    DWORD size = FormatMessageW(
        FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
        nullptr,
        errorCode,
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        reinterpret_cast<LPWSTR>(&buffer),
        0,
        nullptr
    );

    if (size == 0 || !buffer) {
        return fmt::format("Unknown Win32 Error (Code: 0x{:04X})", errorCode);
    }
    std::string result = encoding::to_utf8(buffer);
    LocalFree(buffer);
    result.erase(result.find_last_not_of(" \n\r\t") + 1);
    return fmt::format("{} (Code: 0x{:04X})", result, errorCode);
}

} // namespace bix::win32::utils