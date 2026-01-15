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

#include "text_format.h"

#include "../../window/backends/win32/win32_encoding.h"
#include "bixlib/utils/fmt_bix.h"

#include <fmt/base.h>

namespace bix {
using namespace std;

void debugTo(IDWriteTextLayout* layout) {
    BIX_UNUSED(layout)
    // fmt::println("------------------");
    // DWRITE_TEXT_METRICS textMetrics{};
    // layout->GetMetrics(&textMetrics);
    // FLOAT minWidth = 0.f;
    // layout->DetermineMinWidth(&minWidth); // 13.7988 82.7929688/6
    //
    // auto w = layout->GetMaxWidth();
    // auto h = layout->GetMaxHeight();
    //
    // fmt::println("max w:{} h:{}", w, h);
    //
    // fmt::println("layout min:{} w:{} h:{}", minWidth, textMetrics.width, textMetrics.height);
    // fmt::println("layout line:{} wi:{}", textMetrics.lineCount, textMetrics.widthIncludingTrailingWhitespace);
    // fmt::println("layout left:{} top:{}", textMetrics.left, textMetrics.top);
    //
    // fmt::println("layout lw:{} lh:{}", textMetrics.layoutWidth, textMetrics.layoutHeight);
}

void throw_if_fail(HRESULT hr, const string& msg = "") {
    if (hr != S_OK) {
        auto t = fmt::format("D2DTextFormat operation({}) failed,code:{}", msg, hr);
        throw runtime_error(t);
    }
}

DWRITE_FONT_STYLE convert_as_D2DFontStyle(FontStyle src) {
    switch (src) {
    case FontStyle::Normal: return DWRITE_FONT_STYLE_NORMAL;
    case FontStyle::Oblique: return DWRITE_FONT_STYLE_OBLIQUE;
    case FontStyle::Italic: return DWRITE_FONT_STYLE_ITALIC;
    default: return DWRITE_FONT_STYLE_NORMAL;
    }
}

DWRITE_WORD_WRAPPING convert_as_D2DWordWrapping(WordWrapping src) {
    switch (src) {
    case WordWrapping::Wrap: return DWRITE_WORD_WRAPPING_WRAP;
    case WordWrapping::NoWrap: return DWRITE_WORD_WRAPPING_NO_WRAP;
    case WordWrapping::Character: return DWRITE_WORD_WRAPPING_CHARACTER;
    case WordWrapping::EmergencyBreak: return DWRITE_WORD_WRAPPING_EMERGENCY_BREAK;
    case WordWrapping::WholeWord: return DWRITE_WORD_WRAPPING_WHOLE_WORD;
    default: return DWRITE_WORD_WRAPPING_WRAP;
    }
}

D2DTextFormat::D2DTextFormat(IDWriteFactory* factory, uintptr_t scopeId, float density)
    : mFactory(factory)
    , mScopeId(scopeId)
    , mDisplayDensity(density) {
    BIX_UNUSED(mDisplayDensity)
}

void D2DTextFormat::setFontFamily(const std::string& name) {
    if (name.empty() || name == mFontFamilyName) { return; }
    mFontFamilyName = name;

    if (mLayout) {
        auto s = utf8_to_wstring(name);
        throw_if_fail(mLayout->SetFontFamilyName(s.c_str(), mTmpTextRange));
    }
}

void D2DTextFormat::setTextSize(float size) {
    if (size < 0.01) { return; }
    mTextSize = size;
    if (mLayout) { throw_if_fail(mLayout->SetFontSize(mTextSize, mTmpTextRange)); }
}

void D2DTextFormat::setFontWeight(int weight) {
    if (weight < 1 || weight > 999 || mFontWeight == weight) { return; }
    mFontWeight = weight;
    if (mLayout) { throw_if_fail(mLayout->SetFontWeight(static_cast<DWRITE_FONT_WEIGHT>(mFontWeight), mTmpTextRange)); }
}

void D2DTextFormat::setWordWrapping(WordWrapping wrap) {
    if (wrap == WordWrapping::Wrap) { return; }
    mWordWrapping = wrap;
    if (mLayout) { throw_if_fail(mLayout->SetWordWrapping(convert_as_D2DWordWrapping(wrap))); }
}

void D2DTextFormat::setFontStyle(FontStyle style) {
    if (style == mFontStyle) { return; }
    mFontStyle = style;
    if (mLayout) { throw_if_fail(mLayout->SetFontStyle(convert_as_D2DFontStyle(style), mTmpTextRange)); }
}

void D2DTextFormat::setMaxSize(const UISize& maxSize) {
    if (mMaxSize == maxSize) { return; }
    mMaxSize = maxSize;
    if (mMaxSize.width < 0) { mMaxSize.width = 0; }
    if (mMaxSize.height < 0) { mMaxSize.height = 0; }
    if (mLayout) {
        throw_if_fail(mLayout->SetMaxWidth(numeric_cast<float>(mMaxSize.width)));
        throw_if_fail(mLayout->SetMaxHeight(numeric_cast<float>(mMaxSize.height)));
        debugTo(mLayout.get());
    }
}

void D2DTextFormat::setMaxWidth(int w) {
    if (mMaxSize.width == w) { return; }
    if (w < 0) { w = 0; }
    mMaxSize.width = w;
    if (mLayout) { throw_if_fail(mLayout->SetMaxWidth(numeric_cast<float>(mMaxSize.width))); }
}

void D2DTextFormat::setMaxHeight(int h) {
    if (mMaxSize.height == h) { return; }
    if (h < 0) { h = 0; }
    mMaxSize.height = h;
    if (mLayout) { throw_if_fail(mLayout->SetMaxHeight(numeric_cast<float>(mMaxSize.height))); }
}

void D2DTextFormat::setText(const std::string& text) {
    if (text == mText) { return; }
    mText = text;
    release();
}

void D2DTextFormat::setTrimming(TextTrimming trimming) {
    if (mTextTrimming == trimming) { return; }
    mTextTrimming = trimming;
    setupTrimming();
}

bool D2DTextFormat::testCast(uintptr_t scope, long castId) const noexcept {
    if (scope != mScopeId || D2DTextFormat_CAST_ID != castId) { return false; }
    return true;
}

D2DTextFormat* D2DTextFormat::prepare() {
    if (!mLayout) { create(); }
    return this;
}

IDWriteTextLayout* D2DTextFormat::layout() const noexcept {
    return mLayout.get();
}

void D2DTextFormat::create() {
    IDWriteTextFormat* format = nullptr;
    auto hr = mFactory->CreateTextFormat(
        utf8_to_wstring(mFontFamilyName).c_str(), // Font family name.
        nullptr,                                  // Font collection (NULL sets it to use the system font collection).
        static_cast<DWRITE_FONT_WEIGHT>(mFontWeight), // DWRITE_FONT_WEIGHT
        convert_as_D2DFontStyle(mFontStyle),          // DWRITE_FONT_STYLE
        DWRITE_FONT_STRETCH_NORMAL,                   // DWRITE_FONT_STRETCH
        mTextSize,
        utf8_to_wstring(mLocale).c_str(),
        &format
    );
    throw_if_fail(hr);

    mFormat = DWriteTextFormatPtr(format);

    auto text = utf8_to_wstring(mText);
    IDWriteTextLayout* layout = nullptr;
    hr = mFactory->CreateTextLayout(
        text.c_str(),
        numeric_cast<UINT32>(text.size()),
        mFormat.get(),
        numeric_cast<float>(mMaxSize.width),
        numeric_cast<float>(mMaxSize.height),
        &layout
    );
    throw_if_fail(hr);

    debugTo(layout);

    mLayout = DWriteTextLayoutPtr(layout);
    throw_if_fail(mLayout->SetWordWrapping(convert_as_D2DWordWrapping(mWordWrapping)));

    setupTrimming();

    debugTo(layout);
}

void D2DTextFormat::release() {
    mLayout = nullptr;
    mFormat = nullptr;
    mTrimmingSign = nullptr;
}

void D2DTextFormat::setupTrimming() {
    if (!mLayout) { return; }

    switch (mTextTrimming) {
    case TextTrimming::None:
        mDWTrim.granularity = DWRITE_TRIMMING_GRANULARITY_NONE;
        mLayout->SetTrimming(&mDWTrim, nullptr);
        break;
    case TextTrimming::Ellipsis:
        mDWTrim.granularity = DWRITE_TRIMMING_GRANULARITY_CHARACTER;
        if (!mTrimmingSign) {
            IDWriteInlineObject* trimSign = nullptr;
            mFactory->CreateEllipsisTrimmingSign(mLayout.get(), &trimSign);
            mTrimmingSign = DWInlineObjPtr(trimSign);
        }
        mLayout->SetTrimming(&mDWTrim, mTrimmingSign.get());
        break;
    case TextTrimming::Clip:
        mDWTrim.granularity = DWRITE_TRIMMING_GRANULARITY_CHARACTER;
        mLayout->SetTrimming(&mDWTrim, nullptr);
        break;
    default:
        // TODO custom trimming
        mDWTrim.granularity = DWRITE_TRIMMING_GRANULARITY_CHARACTER;
        mLayout->SetTrimming(&mDWTrim, nullptr);
    }
}
} // namespace bix
