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

#include <fmt/format.h>

#include <windows.h>

namespace bix::win32::utils {

std::string format_win32_error(DWORD errorCode);

struct Win32Error {
    DWORD code;

    Win32Error() : code(GetLastError()) {}

    explicit Win32Error(DWORD c) : code(c) {}
};

inline std::string format_as(Win32Error e) { return format_win32_error(e.code); }

} // namespace bix::win32::utils