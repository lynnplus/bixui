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

#include "window/window_private.h"

#include <bixlib/geometry/rect.h>

using namespace std;

namespace bix {

WindowPrivate::WindowPrivate(Window* w) : mPublic(w) {
    mNative = NativeWindow::create(this);
}

// void WindowPrivate::createWindow() {}

//
// void WindowPrivate::createWindow() {
//     if (!mNativeWindow) {
//         mNativeWindow = NativeWindow::create(mWindow);
//     }
// }
//
void WindowPrivate::setWindowTitle(string_view title) {
    if (mWindowTitle == title) { return; }
    mWindowTitle = title;
    mNative->setTitle(mWindowTitle);

    RectI a{};
    RectF b{};

    Rect m(a);
    Rect c(b);

    RectI p(b);

    if (m == c) {}

    if (c.isEmpty()) {}
    if (m.isEmpty()) {}
    BIX_UNUSED(p)
}

//
// void WindowPrivate::setWindowState(Window::State state) {
//     if (mWindow->uniqueId().empty()) {
//         return;
//     }
//     BIX_UNUSED(state);
//     mNativeWindow->setVisible(true);
// }

bool WindowPrivate::queryNativeInfo(NativeWindowInfo& info) const {
    return mNative->queryNativeInfo(info);
}

std::string WindowPrivate::title() {
    return mWindowTitle;
}

// void WindowPrivate::handlePaint(const UIRect& rect) {
//     if (!mRootLayout) {
//         return;
//     }
//     BIX_UNUSED(rect);
//     // fmt::println("update={}",rect);
//     if (!mCanvas) {
//         auto engine = RenderEngine::from(RenderEngine::Direct2D);
//         mCanvas = engine->createCanvas(*mWindow);
//     }
//
//     mRootLayout->measure(*mCanvas, mWindowClientRect.size(), {-1, -1});
//
//     mRootLayout->layout({0, 0, mWindowClientRect.size()});
//
//     mCanvas->beginDraw();
//     if (mTmpEraseBkFlag) {
//         mCanvas->clear(colors::White);
//         mTmpEraseBkFlag = false;
//     }
//     //
//
//     mRootLayout->draw(*mCanvas);
//
//     // mRootLayout->onDraw(mCanvas.get());
//     mCanvas->endDraw();
//
//     mNativeWindow->validateRect({});
// }
//
// void WindowPrivate::handleMouseEvent(WindowEventType ttype, const Point<int>& p) {
//
//     if (!mRootLayout) {
//         return;
//     }
//
//     // enter move leave
//     //  fmt::println("event={} {}", p.x, p.y);
//
//     auto now = chrono::steady_clock::now();
//     auto ts = std::chrono::duration_cast<std::chrono::seconds>(now.time_since_epoch()).count();
//
//     MouseEvent event(p, mLastMousePos);
//     event.ttype = ttype;
//     event.timestamp = ts;
//
//     mRootLayout->dispatchMouseMoveEvent(event);
// }
//
// intptr_t WindowPrivate::nativeHandle() const {
//     return mNativeWindow->nativeHandle();
// }
//
// void WindowPrivate::setRootLayout(LayoutPtr root) {
//     mRootLayout = std::move(root);
//     mRootLayout->setWindow(this->mWindow);
//     mRootLayout->setSize(FullParent, FullParent);
//     mRootLayout->setMaximumSize({-1, -1});
//     mRootLayout->setMinimumSize({-1, -1});
// }
//
// void WindowPrivate::setWindowSize(const UILength& w, const UILength& h) {
//     mWindowSize = {w, h};
//     mWindowRect.setSize(w.fixed(), h.fixed());
// }
//
// void WindowPrivate::setWindowClientSize(const UILength& w, const UILength& h) {
//     mWindowClientSize = {w, h};
//     mWindowClientRect.setSize(w.fixed(), h.fixed());
// }
//
// void WindowPrivate::handleEvent(WindowEvent& event) {
//     switch (event.ttype) {
//     case WindowEventType::PaintEvent:
//         handlePaint(event.data.rect);
//         break;
//     case WindowEventType::CloseEvent:
//         event.handled = mWindow->onClose();
//         break;
//     case WindowEventType::DestroyEvent:
//         mWindow->onDestroy();
//         break;
//     case WindowEventType::ClientResizeEvent:
//         handleResizeEvent(event.data.size);
//         break;
//     case WindowEventType::EraseBkgndEvent:
//         mTmpEraseBkFlag = true;
//         break;
//     case WindowEventType::MouseLButtonDownEvent:
//     case WindowEventType::MouseLButtonUpEvent:
//     case WindowEventType::MouseMoveEvent:
//         handleMouseEvent(event.ttype, event.data.point);
//         break;
//     default:
//         break;
//     }
// }
//
// string WindowPrivate::windowTitle() {
//     return mWindowTitle;
// }
//
// const UIRect& WindowPrivate::windowClientRect() const noexcept {
//     return mWindowClientRect;
// }
//
// const UIRect& WindowPrivate::windowRect() const noexcept {
//     return mWindowRect;
// }
//
// void WindowPrivate::setUniqueId(const std::string& id) {
//     if (mID.empty() && !id.empty()) {
//         mID = id;
//     }
// }
//
// DisplayPtr WindowPrivate::display() const {
//     if (mNativeWindow) {
//         return mNativeWindow->display();
//     }
//     return WindowManager::defaultDisplay();
// }
//
// // void WindowPrivate::setDisplayStrategy(DisplayStrategyCRef strategy) {
// //     if (strategy) {
// //         mDisplayStrategy = strategy;
// //     }
// // }
//
// DisplayStrategyPtr WindowPrivate::displayStrategy() {
//     if (mDisplayStrategy) {
//         return mDisplayStrategy;
//     }
//     return WindowManager::displayStrategy();
// }
//
// void WindowPrivate::invalidateRect(const UIRect& rect) {
//     if (mNativeWindow) {
//         mNativeWindow->invalidateRect(rect);
//     }
// }
//
void WindowPrivate::requestClose(CloseRequest::Reason reason) {
    CloseRequest request(reason);
    mPublic->onClose(request);
    // ReSharper disable once CppDFAConstantConditions
    if (!request.isCancelled()) { this->performDestroy(); }
}

void WindowPrivate::performDestroy() {
    mNative->destroyNative();
}

ScreenPtr WindowPrivate::screen() const {
    return mNative->getScreen();
}

//
// void WindowPrivate::handleResizeEvent(const SizeF& size) {
//     BIX_UNUSED(size)
//
//     // fmt::println("handleResizeEvent {} {} {}",size.width,size.height,mWindowClientRect);
//     if (!size.isValid()) {
//         return;
//     }
//     mWindowClientRect.setSize(size.aligned());
//
//     // client area size
//     //  auto newSize=size.scale(1.f/mDisplayMetric->scaleFactor()).toAligned();
//     //  // mWindowRect.setSize(newSize);
//     //
//     //  if (mNativeWindow) {
//     //      mNativeWindow->clientRect(mWindowClientRect);
//     //  }
//     //
//     //  // fmt::println("test {} {}",mWindowClientRect.width(), mWindowClientRect.height());
//     //  fmt::println(">>event {} {}",size.width, size.height);
//     //  fmt::println(">>setva {} {}",newSize.width, newSize.height);
//     //  fmt::println(">>caler {} {} {}
//     //  {}",mWindowClientRect.x1,mWindowClientRect.y1,mWindowClientRect.x2,mWindowClientRect.y2);
//     if (mCanvas) {
//         // mCanvas->resize(size);
//         if (mRootLayout) {
//             mRootLayout->layout({0, 0, mWindowClientRect.size()});
//         }
//     }
// }
} // namespace bix
