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

#include "bixlib/geometry.h"

namespace bix {
enum class WindowEventType {
    NilEvent,
    PaintEvent,
    CloseEvent,
    DestroyEvent,
    ClientResizeEvent,
    EraseBkgndEvent,
    MouseMoveEvent,
    MouseLButtonDownEvent,
    MouseLButtonUpEvent,
    MouseRButtonDownEvent,
    MouseRButtonUpEvent
};

class WindowEvent {
public:
    union WindowEventData {
        Point<int> point;
        Size<float> size;
        Rect<int> rect;
    };

    WindowEventType ttype = WindowEventType::NilEvent;
    WindowEventData data{};
    bool handled = false;
};

class MouseEvent {
public:
    MouseEvent(const Point<int>& pos, const Point<int>& lastPos) : mPosition(pos), mLastPosition(lastPos) {}

    const Point<int>& position() const { return mPosition; }

    WindowEventType ttype = WindowEventType::NilEvent;
    int64_t timestamp = 0; // using std::chrono::steady_clock::now()

private:
    Point<int> mPosition;
    Point<int> mLastPosition;
};
} // namespace bix