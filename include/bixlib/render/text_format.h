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
#include <string>

#include "unsafe.h"
#include "bixlib/geometry/rectangle.h"


namespace bix {
enum class TextDirection {
    LEFT_TO_RIGHT = 0,
    RIGHT_TO_LEFT = 1,
    TOP_TO_BOTTOM = 2,
    BOTTOM_TO_TOP = 3,
};


enum class WordWrapping {
    WRAP,
    NO_WRAP,
    EMERGENCY_BREAK,
    WHOLE_WORD,
    CHARACTER
};

enum class FontStyle {
    NORMAL,
    OBLIQUE,
    ITALIC
};

enum class TextAlignment {
    DWRITE_TEXT_ALIGNMENT_LEADING,
    DWRITE_TEXT_ALIGNMENT_TRAILING,
    DWRITE_TEXT_ALIGNMENT_CENTER,
    DWRITE_TEXT_ALIGNMENT_JUSTIFIED
};


class TextPaint {
public:
    virtual ~TextPaint() = default;
    virtual void setText(const std::string& text) =0;
    virtual void setFontFamily(const std::string& name) =0;
    virtual void setBounds(const UIRect& boundingBox) =0;
    virtual void setTextSize(float size) =0;
    virtual void setFontWeight(int weight) =0;
    virtual void setWordWrapping(WordWrapping wrap) =0;
    virtual void setFontStyle(FontStyle style) =0;

    virtual bool handle(UnsafeHandle& p) noexcept =0;

    virtual const UIRect& bounds() const =0;
};

using TextPaintPtr = std::unique_ptr<TextPaint>;
}
