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
#include "bixlib/render/pen.h"
#include "brush.h"

namespace bix {

constexpr static long D2DPen_CAST_ID = 1766411953L;

class D2DPen : public Pen {
public:
    D2DPen(DSolidColorBrushPtr brush, const Color& c, uintptr_t scopeId);

    void setStrokeWidth(int w) override;
    void setColor(const Color& c) override;
    void setMiterLimit(float limit) override;
    void setDashOffset(float dashOffset) override;
    void setLineStyle(LineStyle style) override;
    void setLineJoin(LineJoinStyle lineJoin) override;
    void setLineCap(CapStyle start, CapStyle end, CapStyle dash) override;
    void setStartCap(CapStyle start) override;
    void setEndCap(CapStyle end) override;
    void setDashCap(CapStyle dash) override;
    void setCustomDash(const std::vector<float>& dashes) override;
    bool testCast(uintptr_t scope, long castId) const noexcept override;
    int strokeWidth() const noexcept override;
    const Color& color() const noexcept override;

    D2DPen* prepare();
    ID2D1Brush* brush() const noexcept;
    ID2D1StrokeStyle* strokeStyle() const noexcept;

protected:
    void createStrokeStyle();
    bool shouldCreate() const;

private:
    DSolidColorBrushPtr mBrush = nullptr;
    Color mColor;
    const uintptr_t mScopeId;
    int mStrokeWidth = 0;
    DStrokeStylePtr mStrokeStyle = nullptr;

    // default
    LineStyle mLineStyle{LineStyle::Solid};
    CapStyle mStartCap{CapStyle::Flat};
    CapStyle mEndCap{CapStyle::Flat};
    CapStyle mDashCap{CapStyle::Flat};
    LineJoinStyle mJoinStyle{LineJoinStyle::Miter};
    std::vector<float> mDashes;
    float mMiterLimit = 10.0f;
    float mDashOffset = 0.0f;
};
} // namespace bix