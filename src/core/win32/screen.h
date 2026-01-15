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
#include "../../../include/bixlib/window/screen.h"

#include <windows.h>

namespace bix {

class WinDisplay : public Display {
public:
    [[nodiscard]] const std::string& deviceName() const noexcept override;
    [[nodiscard]] bool isValid() const noexcept override;
    [[nodiscard]] const UISize& resolution() const noexcept override;
    [[nodiscard]] const UISize& size() const noexcept override;
    [[nodiscard]] int refreshRate() const noexcept override;
    [[nodiscard]] int baseDPI() const noexcept override;
    [[nodiscard]] int dpi() const noexcept override;

    [[nodiscard]] bool isDefault() const noexcept override;

    bool update(HWND hwnd);

    void invalidate();

private:
    const int mBaseDpi = 96;
    HWND mHwnd = nullptr;
    HMONITOR mMonitor = nullptr;
    int mDpi = 96; // USER_DEFAULT_SCREEN_DPI
    std::string mDeviceName{};
    UISize mResolution{};
    UISize mSize{};
    bool mIsPrimary = false;
    bool mIsValid = false;
    int mRefreshRate = 0;
};
} // namespace bix