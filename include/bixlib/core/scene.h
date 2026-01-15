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

#include <bixlib/core/widget_host.h>
#include <bixlib/widgets/widget.h>

namespace bix {

class BIX_PUBLIC Scene : public ViewParent {
public:
    explicit Scene(WidgetHost* host);

private:
    void performLayout() {
        // if (!mRoot)
        //     return;
        // mRoot->onMeasure(Constraints::Loose(mWindowSize));
        // mRoot->onLayout(Rect{0, 0, mWindowSize.w, mWindowSize.h});
    }

    void performPaint(Canvas& canvas) {
        // if (mRoot)
        //     mRoot->onPaint(canvas);
    }

    WidgetHost* mHost;
    WidgetPtr mRoot;
    Size mWindowSize;
    bool mDirtyLayout = true;
    bool mDirtyPaint = true;
};

using ScenePtr = std::unique_ptr<Scene>;
} // namespace bix