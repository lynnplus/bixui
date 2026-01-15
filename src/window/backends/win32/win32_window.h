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
#include "window/native_window.h"

#include <windows.h>

namespace bix::win32 {

class Win32Window : public NativeWindow {
public:
    explicit Win32Window(Host* host) : mHost(host) {}

    void cc();
    void createNative() override;
    void destroyNative() override;
    bool queryNativeInfo(NativeWindowInfo& info) const override;
    void setTitle(std::string_view title) override;
    ScreenPtr getScreen() const override;

protected:
    virtual const wchar_t* className() const noexcept;

private:
    Host* mHost;
    // WindowCreateParams mParams{};
    HWND mHwnd = nullptr;

    static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp);

    LRESULT handleMessage(UINT msg, WPARAM wp, LPARAM lp);
};

} // namespace bix::win32