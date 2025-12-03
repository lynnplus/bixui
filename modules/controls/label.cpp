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

#include "bixlib/controls/label.h"

#include "bixlib/control_names.h"
#include "bixlib/render/engine.h"
#include "bixlib/utils/fmt_bix.h"

namespace bix {

const std::string& Label::className() const noexcept { return names::ClsNameLabel; }

void Label::setText(const std::string& str) { mText = str; }

void Label::setTextSize(int size) { mTextSize = size; }

void Label::setTextLines(int maxLines) {
    if (mMaxLines != maxLines && maxLines > 0) {
        mMaxLines = maxLines;
    }
}

void Label::onLayout(const UIRect& rect) {
    BIX_UNUSED(rect)
    if (mTextPaint) {
        mTextPaint->setMaxSize(mTextBox.size());
    }
}

void Label::onDraw(Canvas& canvas) {
    Control::onDraw(canvas);

    if (!mBrush) {
        mBrush = canvas.createColorBrush(mTextColor);
    }
    if (!mTextPaint) {
        setupTextPaint(canvas);
    }
    canvas.drawText(mTextBox.lt(), *mTextPaint, *mBrush);
}

void Label::discardCanvas() {
    Control::discardCanvas();
    mBrush = nullptr;
    mTextPaint = nullptr;
}

void Label::drawText() {}

void Label::setupTextPaint(Canvas& canvas) {
    mTextPaint = canvas.createTextPaint();
    mTextPaint->setText(mText);
    mTextPaint->setTextSize(numeric_cast<float>(mTextSize));
    mTextPaint->setMaxSize(mTextBox.size());
}

void Label::onMeasure(Canvas& canvas, const UISize& available, const UISize& max) {

    if (mText.empty()) {
        // TODO measure background-image
        setMeasuredSize({0, 0});
        return;
    }
    if (available.width == 0 && available.height == 0) {
        mTextPaint->setMaxSize({0, 0});
        setMeasuredSize({0, 0});
        return;
    }

    auto result = mSize.get(available);
    auto pad = getPaddingWithForeground();

    if (!mTextPaint) {
        setupTextPaint(canvas);
    }
    mTextPaint->setMaxSize(result - pad);

    TextMetrics metrics{};
    canvas.measureText(*mTextPaint, metrics);

    if (result.width < 0) {
        result.width = metrics.minWidth + pad.totalX();
    }

    result.width = std::max(result.width, mMinSize.width);
    if (available.width >= 0 && result.width > available.width) {
        result.width = available.width;
    }
    if (max.width >= 0) {
        result.width = std::min(result.width, max.width);
    }

    int remaining = std::max(0, result.width - pad.totalX());
    mTextBox.setWidth(std::min(metrics.minWidth, remaining));

    if (result.height < 0) {
        mTextPaint->setMaxWidth(mTextBox.width());
        canvas.measureText(*mTextPaint, metrics);
        result.height = metrics.height + pad.totalY();
    }
    result.height = std::max(result.height, mMinSize.height);
    if (available.height >= 0 && result.height > available.height) {
        result.height = available.height;
    }
    if (max.height >= 0) {
        result.height = std::min(result.height, max.height);
    }
    remaining = std::max(0, result.height - pad.totalY());
    mTextBox.setHeight(std::min(metrics.height, remaining));

    setMeasuredSize(result);
    // fmt::println("label::onMeasure: {} {}", result,max);
}
} // namespace bix