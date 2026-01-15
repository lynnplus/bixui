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

#include "bixlib/graphics/brush.h"

#include "direct2d.h"

namespace bix {

constexpr static long D2DBasicBrush_CAST_ID = 1766407102L;

template <DerivedFrom<Brush> Derived = Brush>
class D2DBasicBrush : public Derived {
public:
    void setOpacity(float opacity) override { mBrush->SetOpacity(opacity); }

    float opacity() const noexcept override { return mBrush->GetOpacity(); }

    bool testCast(uintptr_t scope, long castId) const noexcept override {
        if (scope != mScopeId || D2DBasicBrush_CAST_ID != castId) { return false; }
        return true;
    }

    ID2D1Brush* native() const noexcept { return mBrush; }

protected:
    D2DBasicBrush(ID2D1RenderTarget* renderTarget, ID2D1Brush* brush, uintptr_t scopeId)
        : mRenderTarget(renderTarget)
        , mBrush(brush)
        , mScopeId(scopeId) {}

private:
    ID2D1RenderTarget* mRenderTarget = nullptr;
    ID2D1Brush* mBrush = nullptr;
    const uintptr_t mScopeId;
};

class D2DSolidColorBrush : public D2DBasicBrush<ColorBrush> {
public:
    D2DSolidColorBrush(DSolidColorBrushPtr brush, ID2D1RenderTarget* renderTarget, uintptr_t scopeId)
        : D2DBasicBrush(renderTarget, brush.get(), scopeId)
        , mBrushImpl(std::move(brush)) {}

    void setColor(const Color& color) override;
    Color color() const noexcept override;

protected:
    DSolidColorBrushPtr mBrushImpl = nullptr;
};

class D2DLinearGradientBrush {
public:
};

class D2DRadialGradientBrush {
public:
};

class D2DBitmapBrush {
public:
};

} // namespace bix
