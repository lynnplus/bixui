
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

#include "window/backends/win32/class_manager.h"

#include "window/backends/win32/win32_encoding.h"
#include "window/backends/win32/win32_utils.h"

#include <spdlog/spdlog.h>

#include <mutex>
#include <unordered_map>

namespace bix::win32 {

namespace {
std::unordered_map<std::wstring, std::string> s_nameCache;
std::mutex s_mtx;
} // namespace

bool ClassManager::prepare(const wchar_t* clsName, WNDPROC wndProc) {
    std::lock_guard lock(s_mtx);
    if (s_nameCache.contains(clsName)) {
        SPDLOG_TRACE("win32 class '{}' already in cache, skipping.", encoding::to_utf8(clsName));
        return true;
    }
    if (auto it = s_nameCache.find(clsName); it != s_nameCache.end()) {
        SPDLOG_TRACE("win32 class '{}' already in cache, skipping.", it->second);
        return true;
    }

    HINSTANCE hInst = GetModuleHandle(nullptr);
    WNDCLASSEXW wcex{};
    wcex.cbSize = sizeof(wcex);

    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = wndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInst;
    wcex.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = reinterpret_cast<HBRUSH>(COLOR_WINDOWFRAME);
    wcex.lpszMenuName = nullptr;
    wcex.lpszClassName = clsName;
    wcex.hIconSm = LoadIcon(nullptr, IDI_APPLICATION);

    if (RegisterClassExW(&wcex) == 0) {
        if (DWORD err = GetLastError(); err != ERROR_CLASS_ALREADY_EXISTS) {
            spdlog::error("win32 class registration failed! Error:{}", utils::Win32Error{err});
            return false;
        }
        SPDLOG_DEBUG("win32 class() already registered in system.", encoding::to_utf8(clsName));
    }
    std::string utf8Name = encoding::to_utf8(clsName);
    SPDLOG_DEBUG("win32 class({}) registration complete.", utf8Name);
    s_nameCache.emplace(clsName, std::move(utf8Name));
    return true;
}

const char* ClassManager::getClassName(const wchar_t* clsName) {
    std::lock_guard lock(s_mtx);
    if (auto it = s_nameCache.find(clsName); it != s_nameCache.end()) { return it->second.c_str(); }
    return nullptr;
}

} // namespace bix::win32
