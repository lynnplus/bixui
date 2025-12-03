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

#include "bixlib/render/brush.h"
#include "direct2d.h"

namespace bix {

template <DerivedFrom<Brush> Derived>
class D2DBasicBrush : public Derived {
public:
    D2DBasicBrush(ID2D1RenderTarget* rt, uintptr_t id) : mRenderTarget(rt), mId(id) {}

    void setOpacity(float opacity) override { current()->SetOpacity(opacity); }

    float opacity() const noexcept override { return current()->GetOpacity(); }

    bool handle(UnsafeHandle& p) noexcept override {
        if (p.mUniqueId != mId) {
            return false;
        }
        p.set(current());
        return true;
    }

    virtual ID2D1Brush* current() const = 0;

private:
    ID2D1RenderTarget* mRenderTarget = nullptr;
    const uintptr_t mId;
};

class D2DSolidColorBrush : public D2DBasicBrush<ColorBrush> {
public:
    D2DSolidColorBrush(DSolidColorBrushPtr b, ID2D1RenderTarget* rt, uintptr_t id);
    void setColor(const Color& color) override;
    Color color() const noexcept override;
    ID2D1Brush* current() const override;

private:
    DSolidColorBrushPtr mBrushImpl = nullptr;
};

class D2DLinearGradientBrush {
public:
private:
};

class D2DRadialGradientBrush {
public:
};

class D2DBitmapBrush {
public:
};

} // namespace bix
