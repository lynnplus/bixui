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
#include "../widgets/container.h"

namespace bix {
class BIX_PUBLIC LinearLayout : public Container {
public:
    enum class Orientation {
        Horizontal,
        Vertical
    };
    void setOrientation(bool horizontal);

protected:
    void onLayout(const UIRect& pos) override;
    void onMeasure(Canvas& canvas, const UISize& available, const UISize& max) override;

    void measureVertical(Canvas& canvas, const UISize& available, const UISize& max);

private:
    // orientation="horizontal
    bool mIsHorizontal = false; // default vertical
};

class BIX_PUBLIC HBox : public LinearLayout {
public:
    Row() : LinearLayout(Orientation::Horizontal) {}
};

class BIX_PUBLIC VBox : public LinearLayout {
public:
    Column() : LinearLayout(Orientation::Vertical) {}
};
} // namespace bix
