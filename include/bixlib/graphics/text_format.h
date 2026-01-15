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
#include "bixlib/geometry/rect.h"

#include <string>

#include "color.h"

namespace bix {
enum class TextDirection {
    LEFT_TO_RIGHT = 0,
    RIGHT_TO_LEFT = 1,
    TOP_TO_BOTTOM = 2,
    BOTTOM_TO_TOP = 3,
};

enum class WordWrapping {
    Wrap,
    NoWrap,
    EmergencyBreak,
    WholeWord,
    Character
};

enum class FontStyle {
    Normal,
    Oblique,
    Italic
};

enum class TextAlignment {
    Leading,
    Trailing,
    Center,
    Justified
};

enum class TextTrimming {
    None, // overflow
    Clip,
    Ellipsis,
    Character, // custom character
};

class TextPaint {
public:
    virtual ~TextPaint() = default;
    virtual void setText(const std::string& text) = 0;
    virtual void setFontFamily(const std::string& name) = 0;
    // virtual void setMaxSize(const UISize& maxSize) = 0;
    virtual void setMaxWidth(int w) = 0;
    virtual void setMaxHeight(int h) = 0;
    virtual void setTextSize(float size) = 0;
    virtual void setFontWeight(int weight) = 0;
    virtual void setWordWrapping(WordWrapping wrap) = 0;
    virtual void setFontStyle(FontStyle style) = 0;
    virtual void setTrimming(TextTrimming trimming) = 0;

    virtual bool testCast(uintptr_t scope, long castId) const noexcept = 0;
};

using TextPaintPtr = std::unique_ptr<TextPaint>;
} // namespace bix
