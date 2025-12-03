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

#include "bixlib/controls/layout.h"
#include "bixlib/core/window_events.h"
#include "native_window.h"

namespace bix {
class WindowPrivate {
public:
    explicit WindowPrivate(Window* w);

    ~WindowPrivate() {}

    void createWindow();

    void setWindowTitle(const std::string& title);
    void setWindowState(Window::State state);

    [[nodiscard]]
    intptr_t nativeHandle() const;
    void setRootLayout(LayoutPtr root);
    void setWindowSize(const UILength& w, const UILength& h);
    void setWindowClientSize(const UILength& w, const UILength& h);

    void handleEvent(WindowEvent& event);

    std::string windowTitle();

    [[nodiscard]] const UIRect& windowClientRect() const noexcept;
    [[nodiscard]] const UIRect& windowRect() const noexcept;

    [[nodiscard]]
    std::string uniqueId() const {
        return mID;
    };

    void setUniqueId(const std::string& id);
    [[nodiscard]] DisplayPtr display() const;
    void setDisplayStrategy(DisplayStrategyCRef strategy);
    DisplayStrategyPtr displayStrategy();

    void invalidateRect(const UIRect& rect);

private:
    friend class Win32Window;

    NativeWindowPtr mNativeWindow = nullptr;
    std::string mID{};
    std::string mWindowTitle{};

    Window* mWindow = nullptr;

    LayoutPtr mRootLayout = nullptr;
    CanvasPtr mCanvas = nullptr;

    DisplayStrategyPtr mDisplayStrategy = nullptr;

    UIRect mWindowRect{};
    UIRect mWindowClientRect{};

    SpecSize mWindowSize{};
    SpecSize mWindowClientSize{};

    Point<int> mLastMousePos{-1, -1};

    bool mTmpEraseBkFlag = false;

    void handleResizeEvent(const SizeF& size);
    void handlePaint(const UIRect& rect);
    void handleMouseEvent(WindowEventType ttype, const Point<int>& p);
};
} // namespace bix