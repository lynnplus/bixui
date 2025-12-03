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
#include "bixlib/controls/control.h"

namespace bix {
class BIX_PUBLIC Label : public Control {
public:
    const std::string& className() const noexcept override;

    void setText(const std::string& str);
    void setTextSize(int size);
    // void setTextAlignment();

    void setTextLines(int maxLines);

protected:
    void onLayout(const UIRect& rect) override;

public:
    void onDraw(Canvas& canvas) override;
    void discardCanvas() override;

protected:
    ColorBrushPtr mBrush = nullptr;
    TextPaintPtr mTextPaint = nullptr;
    Color mTextColor = colors::Black;

    UIRect mTextBox{0, 0,0,0};//text layout bounds box
    std::string mText{};
    int mTextSize = 12;
    int mMaxLines = 0; // unlimit

    void drawText();

    void setupTextPaint(Canvas& canvas);

    void onMeasure(Canvas& canvas, const UISize& available, const UISize& max) override;
};
} // namespace bix
