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

#include "bixlib/window/window.h"

#include "window/window_private.h"

#include <fmt/format.h>

namespace bix {

Window::Window() : mPrivate(std::make_unique<WindowPrivate>(this)) {}

// NOLINT(*-use-equals-default)
Window::~Window() {
    fmt::println("Window::~Window()");
}

void Window::show() {
    // mPrivate->createWindow();
    // mPrivate->setWindowState(State::WindowNormal);

    fmt::println("window show");

    // const auto *platformIntegration = QGuiApplicationPrivate::platformIntegration();
    // Qt::WindowState defaultState = platformIntegration->defaultWindowState(d_func()->windowFlags);
    // if (defaultState == Qt::WindowFullScreen)
    //     showFullScreen();
    // else if (defaultState == Qt::WindowMaximized)
    //     showMaximized();
    // else
    //     showNormal();
    //
    //
    // mPrivate->test();
}

bool Window::queryNativeInfo(NativeWindowInfo& info) const {
    return mPrivate->queryNativeInfo(info);
}

// void Window::resize(const UILength& w, const UILength& h) { mPrivate->setWindowSize(w, h); }

// void Window::resize(int w, int h) { resize(UILength(w), UILength(h)); }

void Window::close() {
    mPrivate->requestClose(CloseRequest::Reason::Programmatic);
}

void Window::destroy() {
    mPrivate->performDestroy();
}

// void Window::setClientSize(const UILength& w, const UILength& h) { mPrivate->setWindowClientSize(w, h); }

// void Window::setClientSize(int w, int h) { setClientSize(UILength(w), UILength(h)); }

void Window::moveToCenter() {}

void Window::setTitle(std::string_view title) {
    mPrivate->setWindowTitle(title);
}

// void Window::invalidateRect(const UIRect& rect) { mPrivate->invalidateRect(rect); }

// WindowPrivate* Window::getImpl() const { return mPrivate.get(); }

std::string Window::title() const {
    return mPrivate->title();
}

// std::string Window::uniqueId() const { return mPrivate->uniqueId(); }

// void Window::setUniqueId(const std::string& id) { return mPrivate->setUniqueId(id); }

ScreenPtr Window::screen() const {
    return mPrivate->screen();
}

// void Window::setLayout(std::unique_ptr<Layout> root) { mPrivate->setRootLayout(std::move(root)); }
} // namespace bix
