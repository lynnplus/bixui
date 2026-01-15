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

#include <bixlib/core/layout_types.h>
#include <bixlib/core/length.h>
#include <bixlib/export_macro.h>
#include <bixlib/window/screen.h>

#include <memory>

namespace bix {
class WindowPrivate;
struct NativeWindowInfo;

class BIX_PUBLIC CloseRequest {
public:
    enum class Reason {
        UserAction,
        Programmatic,
        SystemShutdown,
        AppExit
    };

    explicit CloseRequest(Reason reason) : mReason(reason) {}

    Reason reason() const;

    void cancel() { mCancelled = true; }

    bool isCancelled() const { return mCancelled; }

private:
    Reason mReason;
    bool mCancelled = false;
};

class BIX_PUBLIC Window {
public:
    enum class State : uint8_t {
        Hidden = 0,
        Normal = 1 << 0,
        Minimized = 1 << 1,
        Maximized = 1 << 2,
        FullScreen = 1 << 3,
    };

    Window();
    virtual ~Window();

    void show();
    void hide();
    void close();
    void destroy();

    void minimize();
    void maximize();
    void setFullScreen(bool enable);
    void toggleFullScreen();

    std::string title() const;
    void setTitle(std::string_view title);

    void setName(std::string_view name);
    std::string name() const;

    void setSize(const LayoutSize& size);
    LayoutSize size() const;
    SizeI pixelSize() const;

    void setWindowSize(const LayoutSize& size);
    LayoutSize windowSize() const;
    SizeI windowPixelSize() const;

    void resize(Length w, Length h) { setWindowSize({w, h}); }

    State state() const;
    bool isActive() const;
    bool isMinimized() const;
    bool isMaximized() const;

    bool isFullScreen() const { return state() == State::FullScreen; }

    void moveToCenter();

    [[nodiscard]]
    bool queryNativeInfo(NativeWindowInfo& info) const;
    /**
     * Returns the screen where the majority of the window is currently located.
     */
    [[nodiscard]]
    ScreenPtr screen() const;

protected:
    // listening
    virtual void onBuildFinish() {}

    virtual void onCreate() {}

    virtual void onAttach() {}

    virtual void onShow() {}

    virtual void onResize(const SizeI& size) { BIX_UNUSED(size) }

    virtual void onClose(CloseRequest& request) { BIX_UNUSED(request) }

    virtual void onDestroy() {}

private:
    friend class WindowPrivate;
    std::unique_ptr<WindowPrivate> mPrivate;
};
} // namespace bix