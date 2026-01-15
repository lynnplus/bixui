
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

#include "window/backends/win32/win32_screen.h"

#include "window/backends/win32/win32_encoding.h"

using namespace std;

namespace bix::win32 {

Win32Screen::Win32Screen(HMONITOR monitor) : mMonitor(monitor) {}

string Win32Screen::id() const noexcept {
    return mID;
}

std::string Win32Screen::name() const noexcept {
    return mFriendlyName;
}

std::string Win32Screen::deviceName() const noexcept {
    return mDeviceName;
}

bool Win32Screen::isAvailable() const noexcept {
    MONITORINFOEXW info;
    info.cbSize = sizeof(MONITORINFOEXW);
    return GetMonitorInfoW(mMonitor, &info) != 0;
}

bool Win32Screen::isPrimary() const noexcept {
    MONITORINFO mi{};
    mi.cbSize = sizeof(MONITORINFO);
    if (GetMonitorInfoW(mMonitor, &mi)) { return (mi.dwFlags & MONITORINFOF_PRIMARY) != 0; }
    return false;
}

Point Win32Screen::position() const noexcept {
    MONITORINFO mi{};
    mi.cbSize = sizeof(MONITORINFO);
    if (GetMonitorInfoW(mMonitor, &mi)) {
        return {static_cast<float>(mi.rcMonitor.left), static_cast<float>(mi.rcMonitor.top)};
    }
    return {0, 0};
}

Size Win32Screen::physicalSize() const noexcept {
    MONITORINFO mi{};
    mi.cbSize = sizeof(MONITORINFO);
    if (GetMonitorInfoW(mMonitor, &mi)) {
        return {mi.rcMonitor.right - mi.rcMonitor.left, mi.rcMonitor.bottom - mi.rcMonitor.top};
    }
    return {0, 0};
}

Size Win32Screen::size() const noexcept {

    // Size phys = physicalSize();
    // float scale = scaleFactor();
    // return {static_cast<int>(phys.width / scale), static_cast<int>(phys.height / scale)};
    return {};
}

Rect Win32Screen::workArea() const noexcept {
    MONITORINFO mi{};
    mi.cbSize = sizeof(MONITORINFO);
    // if (::GetMonitorInfoW(mMonitor, &mi)) {
    //     return {mi.rcWork.left, mi.rcWork.top, mi.rcWork.right - mi.rcWork.left, mi.rcWork.bottom - mi.rcWork.top};
    // }
    return {};
}

float Win32Screen::scaleFactor() const noexcept {
    return 0.f;
}

int Win32Screen::refreshRate() const noexcept {
    // MONITORINFOEXW info;
    // if (get_monitor_info(m_hMonitor, info)) {
    //     DEVMODEW dm = {sizeof(dm)};
    //     if (::EnumDisplaySettingsW(info.szDevice, ENUM_CURRENT_SETTINGS, &dm)) {
    //         return static_cast<int>(dm.dmDisplayFrequency);
    //     }
    // }
    return 0;
}

int Win32Screen::dpi() const noexcept {
    // UINT x, y;
    // if (SUCCEEDED(GetDpiForMonitor(m_hMonitor, MDT_EFFECTIVE_DPI, &x, &y))) {
    //     return static_cast<int>(x);
    // }
    return standardDPI();
}

int Win32Screen::standardDPI() const noexcept {
    return USER_DEFAULT_SCREEN_DPI;
}

int Win32Screen::rotation() const noexcept {
    MONITORINFOEXW info{};
    info.cbSize = sizeof(MONITORINFOEXW);
    if (!GetMonitorInfoW(mMonitor, &info)) { return 0; }
    DEVMODEW dm{};
    dm.dmSize = sizeof(dm);
    if (EnumDisplaySettingsW(info.szDevice, ENUM_CURRENT_SETTINGS, &dm)) {
        switch (dm.dmDisplayOrientation) {
        case DMDO_90: return 90;
        case DMDO_180: return 180;
        case DMDO_270: return 270;
        default: return 0;
        }
    }
    return 0;
}

Screen::Data Win32Screen::snapshot() const noexcept {
    Data snapshot = {};
    snapshot.id = mID;
    snapshot.name = mFriendlyName;
    snapshot.deviceName = mDeviceName;
    // TODO impl other
    return snapshot;
}

void Win32Screen::prepareMetadata() {
    if (!mID.empty()) { return; }

    MONITORINFOEXW info{};
    info.cbSize = sizeof(MONITORINFOEXW);
    if (!GetMonitorInfoW(mMonitor, &info)) { return; }

    // deviceName is always available from MONITORINFOEXW
    mDeviceName = encoding::to_utf8(info.szDevice);

    UINT32 pathCount = 0, modeCount = 0;
    if (GetDisplayConfigBufferSizes(QDC_ONLY_ACTIVE_PATHS, &pathCount, &modeCount) == ERROR_SUCCESS) {
        std::vector<DISPLAYCONFIG_PATH_INFO> paths(pathCount);
        std::vector<DISPLAYCONFIG_MODE_INFO> modes(modeCount);
        if (::QueryDisplayConfig(QDC_ONLY_ACTIVE_PATHS, &pathCount, paths.data(), &modeCount, modes.data(), nullptr)
            == ERROR_SUCCESS) {
            for (const auto& path : paths) {
                DISPLAYCONFIG_SOURCE_DEVICE_NAME sourceName = {};
                sourceName.header.type = DISPLAYCONFIG_DEVICE_INFO_GET_SOURCE_NAME;
                sourceName.header.size = sizeof(sourceName);
                sourceName.header.adapterId = path.sourceInfo.adapterId;
                sourceName.header.id = path.sourceInfo.id;

                if (DisplayConfigGetDeviceInfo(&sourceName.header) == ERROR_SUCCESS) {
                    if (std::wcscmp(sourceName.viewGdiDeviceName, info.szDevice) == 0) {
                        DISPLAYCONFIG_TARGET_DEVICE_NAME targetName = {};
                        targetName.header.type = DISPLAYCONFIG_DEVICE_INFO_GET_TARGET_NAME;
                        targetName.header.size = sizeof(targetName);
                        targetName.header.adapterId = path.targetInfo.adapterId;
                        targetName.header.id = path.targetInfo.id;
                        if (DisplayConfigGetDeviceInfo(&targetName.header) == ERROR_SUCCESS) {
                            mID = encoding::to_utf8(targetName.monitorDevicePath);
                            if (targetName.flags.friendlyNameFromEdid) {
                                mFriendlyName = encoding::to_utf8(targetName.monitorFriendlyDeviceName);
                            }
                        }
                    }
                }
            }
        }
    }
    if (mID.empty()) { mID = mDeviceName; }
    if (mFriendlyName.empty()) { mFriendlyName = mDeviceName; }
}
} // namespace bix::win32