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

#include <bixlib/core/widget_host.h>
#include <bixlib/window/window.h>

namespace bix {
class Window;

class WindowPrivate : public WidgetHost, public NativeWindow::Host {
public:
    explicit WindowPrivate(Window* w);

    // void requestLayout() override;

    void createWindow();

    void setWindowTitle(std::string_view title);
    void setWindowState(Window::State state);
    [[nodiscard]]
    bool queryNativeInfo(NativeWindowInfo& info) const;

    // void setRootLayout(LayoutPtr root);
    // void setWindowSize(const UILength& w, const UILength& h);
    // void setWindowClientSize(const UILength& w, const UILength& h);
    //
    // void handleEvent(WindowEvent& event);
    //
    std::string title();
    //
    // [[nodiscard]] const UIRect& windowClientRect() const noexcept;
    // [[nodiscard]] const UIRect& windowRect() const noexcept;
    //
    // [[nodiscard]]
    // std::string uniqueId() const {
    //     return mID;
    // };
    //
    // void setUniqueId(const std::string& id);
    // [[nodiscard]] DisplayPtr display() const;
    // // void setDisplayStrategy(DisplayStrategyCRef strategy);
    // DisplayStrategyPtr displayStrategy();

    // void invalidateRect(const UIRect& rect);
    //
    void scheduleFrame(const Rect& dirtyRect) override;
    void requestLayout() override;
    void captureFocus(Widget* widget) override;
    void requestClose(CloseRequest::Reason reason);
    void performDestroy();
    [[nodiscard]]
    ScreenPtr screen() const;

private:
    // ScenePtr mScene;

    NativeWindowPtr mNative = nullptr;
    Window* mPublic = nullptr;

    std::string mID{};
    std::string mWindowTitle{};
    //
    // LayoutPtr mRootLayout = nullptr;
    // CanvasPtr mCanvas = nullptr;
    //
    // DisplayStrategyPtr mDisplayStrategy = nullptr;
    //
    // UIRect mWindowRect{};
    // UIRect mWindowClientRect{};
    //
    // SpecSize mWindowSize{};
    // SpecSize mWindowClientSize{};
    //
    // Point<int> mLastMousePos{-1, -1};
    //
    // bool mTmpEraseBkFlag = false;

    // void handleResizeEvent(const SizeF& size);
    // void handlePaint(const UIRect& rect);
    // void handleMouseEvent(WindowEventType ttype, const Point<int>& p);
};
} // namespace bix