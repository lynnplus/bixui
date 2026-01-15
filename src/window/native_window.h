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

#include <memory>
#include <string>

namespace bix {

struct NativeWindowInfo;

// struct WindowCreateParams {

//     // std::string mTitle;
//     int mWidth;
//     int mHeight;
//     int mX;
//     int mY;
//
//     // 样式开关
//     bool mIsResizable = true;
//     bool mIsDecorated = true;
//     bool mIsTopmost = false;
//     bool mIsTransparent = false;
//
//     // void* mExternalParent = nullptr;
// };

class NativeWindow {
public:
    class Host {
    public:
        virtual ~Host() = default;
    };

    virtual ~NativeWindow() = default;

    static std::unique_ptr<NativeWindow> create(Host* host);

    static std::unique_ptr<NativeWindow> createDummy();

    virtual void createNative() = 0;
    virtual void destroyNative() = 0;

    virtual bool queryNativeInfo(NativeWindowInfo& info) const = 0;

    virtual void setTitle(std::string_view title) = 0;
    // virtual void setVisible(bool visible) = 0;
    //
    // virtual void invalidateRect(const UIRect& rect) = 0;
    // virtual void validateRect(const UIRect& rect) = 0;
    //
    // virtual void resize(const UISize& size) = 0;
    // virtual void clientRect(UIRect& rect) const = 0;
    // [[nodiscard]] virtual DisplayPtr display() const = 0;

    virtual ScreenPtr getScreen() const = 0;
};

using NativeWindowPtr = std::unique_ptr<NativeWindow>;

} // namespace bix
