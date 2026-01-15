
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

#include <bixlib/window/screen.h>

#include <windows.h>

namespace bix::win32 {

class Win32Screen : public Screen {
public:
    explicit Win32Screen(HMONITOR monitor);
    std::string id() const noexcept override;
    std::string name() const noexcept override;
    std::string deviceName() const noexcept override;
    bool isAvailable() const noexcept override;
    bool isPrimary() const noexcept override;
    Point position() const noexcept override;
    Size physicalSize() const noexcept override;
    Size size() const noexcept override;
    Rect workArea() const noexcept override;
    float scaleFactor() const noexcept override;
    int refreshRate() const noexcept override;
    int dpi() const noexcept override;
    int standardDPI() const noexcept override;
    int rotation() const noexcept override;
    Data snapshot() const noexcept override;

private:
    HMONITOR mMonitor;
    std::string mID;
    std::string mFriendlyName;
    std::string mDeviceName;

    void prepareMetadata();
};

} // namespace bix::win32