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
#include <bixlib/macros.h>
#include <bixlib/widgets/widget.h>

namespace bix {

class BIX_PUBLIC WidgetHost {

public:
    BIX_INTERFACE(WidgetHost)

    virtual void scheduleFrame(const Rect& dirtyRect) = 0;
    virtual void requestLayout() = 0;

    // virtual void setCursor(CursorType type) = 0;
    virtual void captureFocus(Widget* widget) = 0;

    // virtual void postTask(std::function<void()> task) = 0;
};

} // namespace bix
