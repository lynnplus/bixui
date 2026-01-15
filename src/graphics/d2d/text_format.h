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
#include "bixlib/graphics/text_format.h"

#include <dwrite.h>

#include "direct2d.h"

namespace bix {

constexpr static long D2DTextFormat_CAST_ID = 1766413641L;

class D2DTextFormat : public TextPaint {
public:
    explicit D2DTextFormat(IDWriteFactory* factory, uintptr_t scopeId, float density);

    void setFontFamily(const std::string& name) override;
    void setTextSize(float size) override;
    void setFontWeight(int weight) override;
    void setWordWrapping(WordWrapping wrap) override;
    void setFontStyle(FontStyle style) override;
    void setMaxSize(const UISize& maxSize) override;
    void setMaxWidth(int w) override;
    void setMaxHeight(int h) override;
    void setText(const std::string& text) override;
    void setTrimming(TextTrimming trimming) override;
    bool testCast(uintptr_t scope, long castId) const noexcept override;

    D2DTextFormat* prepare();
    IDWriteTextLayout* layout() const noexcept;

private:
    void create();

    void release();

    void setupTrimming();

    IDWriteFactory* mFactory = nullptr;
    uintptr_t mScopeId = 0;
    float mDisplayDensity = 1.f;

    float mTextSize = 12.f;
    std::string mText{};
    Color mTextColor = colors::Black;
    int mFontWeight = DWRITE_FONT_WEIGHT_NORMAL;
    FontStyle mFontStyle = FontStyle::Normal;
    WordWrapping mWordWrapping = WordWrapping::Wrap;
    std::string mFontFamilyName = "Arial";
    // TODO
    std::string mLocale = "en-us";

    DWRITE_TEXT_RANGE mTmpTextRange{0, 0};
    TextTrimming mTextTrimming = TextTrimming::None;

    DWRITE_TRIMMING mDWTrim{DWRITE_TRIMMING_GRANULARITY_CHARACTER, 0, 0};

    UISize mMaxSize = {0, 0};

    DWInlineObjPtr mTrimmingSign = nullptr;
    DWriteTextFormatPtr mFormat = nullptr;
    DWriteTextLayoutPtr mLayout = nullptr;
};
} // namespace bix