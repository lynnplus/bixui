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
#include "bixlib/render/canvas.h"

namespace bix {
class BIX_PUBLIC Drawable {
public:
    virtual ~Drawable() = default;

    void setVisible(bool visible);
    void setBounds(const UIRect& bounds);
    [[nodiscard]]
    const UIRect& bounds() const;


    virtual void draw(Canvas* canvas) =0;
    virtual void setAlpha(int alpha) =0;

    virtual void discardCanvas() =0;

protected:
    bool mVisible = true;
    UIRect mBounds{0, 0, 0, 0};
};

using DrawablePtr = std::unique_ptr<Drawable>;

class BIX_PUBLIC ColorDrawable : public Drawable {
public:
    ColorDrawable() = default;
    explicit ColorDrawable(const Color& color);

    void setAlpha(int alpha) override;
    void draw(Canvas* canvas) override;
    void setColor(const Color& color);
    void discardCanvas() override;

protected:
    Color mColor{};
    ColorBrushPtr mBrush = nullptr;
};
}
