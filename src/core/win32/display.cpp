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

#include "../../window/backends/win32/win32_encoding.h"

#include "screen.h"
#include "win32_api.h"

// https://colinfinck.de/posts/writing-win32-apps-like-its-2020-part-3/

namespace bix {

const std::string& WinDisplay::deviceName() const noexcept {
    return mDeviceName;
}

bool WinDisplay::isValid() const noexcept {
    return mIsValid;
}

const UISize& WinDisplay::resolution() const noexcept {
    return mResolution;
}

const UISize& WinDisplay::size() const noexcept {
    return mSize;
}

int WinDisplay::refreshRate() const noexcept {
    return mRefreshRate;
}

int WinDisplay::baseDPI() const noexcept {
    return mBaseDpi;
}

int WinDisplay::dpi() const noexcept {
    return mDpi;
}

bool WinDisplay::isDefault() const noexcept {
    return mHwnd == nullptr;
}

bool WinDisplay::update(HWND hwnd) {
    mHwnd = hwnd;
    mIsValid = false;
    // The MonitorFromWindow function retrieves a handle to the display monitor
    // that has the largest area of intersection with the bounding rectangle of a specified window.
    mMonitor = MonitorFromWindow(hwnd, MONITOR_DEFAULTTONEAREST);

    MONITORINFOEXW mi;
    mi.cbSize = sizeof(mi);
    if (auto result = GetMonitorInfoW(mMonitor, &mi); result == 0) {
        // TODO fail
        return false;
    }
    mIsPrimary = mi.dwFlags == MONITORINFOF_PRIMARY;
    mDeviceName = wstring_to_utf8(mi.szDevice); // device name
    mDpi = std::max(win32::getWindowDpi(hwnd, mMonitor), 96);

    DEVMODEW devmode = {};
    devmode.dmSize = sizeof(devmode);
    if (EnumDisplaySettingsW(mi.szDevice, ENUM_CURRENT_SETTINGS, &devmode) == 0) {
        // TODO fail
        return false;
    }
    // success
    mResolution.width = numeric_cast<int>(devmode.dmPelsWidth);
    mResolution.height = numeric_cast<int>(devmode.dmPelsHeight);
    mRefreshRate = numeric_cast<decltype(mRefreshRate)>(devmode.dmDisplayFrequency);
    mSize.width = GetSystemMetrics(SM_CXSCREEN);
    mSize.height = GetSystemMetrics(SM_CYSCREEN);
    mIsValid = true;
    return true;
}

void WinDisplay::invalidate() {
    mIsValid = false;
    mHwnd = nullptr;
    mMonitor = nullptr;
}

} // namespace bix
