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

#include "pen.h"

#include "convert-inl.h"

using namespace std;

namespace bix
{
    D2D1_DASH_STYLE convert_toDDashStyle(LineStyle s)
    {
        switch (s)
        {
        case LineStyle::Solid:
            return D2D1_DASH_STYLE_SOLID;
        case LineStyle::Dash:
            return D2D1_DASH_STYLE_DASH;
        case LineStyle::Dot:
            return D2D1_DASH_STYLE_DOT;
        case LineStyle::DashDot:
            return D2D1_DASH_STYLE_DASH_DOT;
        case LineStyle::DashDotDot:
            return D2D1_DASH_STYLE_DASH_DOT_DOT;
        case LineStyle::CustomDash:
            return D2D1_DASH_STYLE_CUSTOM;
        default:
            return D2D1_DASH_STYLE_SOLID;
        }
    }

    D2D1_CAP_STYLE convert_toDCapStyle(CapStyle s)
    {
        switch (s)
        {
        case CapStyle::Flat:
            return D2D1_CAP_STYLE_FLAT;
        case CapStyle::Square:
            return D2D1_CAP_STYLE_SQUARE;
        case CapStyle::Round:
            return D2D1_CAP_STYLE_ROUND;
        case CapStyle::Triangle:
            return D2D1_CAP_STYLE_TRIANGLE;
        default:
            return D2D1_CAP_STYLE_FLAT;
        }
    }

    D2D1_LINE_JOIN convert_toLineJoinStyle(LineJoinStyle s)
    {
        switch (s)
        {
        case LineJoinStyle::Miter:
            return D2D1_LINE_JOIN_MITER;
        case LineJoinStyle::Bevel:
            return D2D1_LINE_JOIN_BEVEL;
        case LineJoinStyle::Round:
            return D2D1_LINE_JOIN_ROUND;
        case LineJoinStyle::MiterOrBevel:
            return D2D1_LINE_JOIN_MITER_OR_BEVEL;
        default:
            return D2D1_LINE_JOIN_MITER;
        }
    }

    D2DPen::D2DPen(DSolidColorBrushPtr brush, ID2D1Factory* factory, const Color& c, uintptr_t id)
        : mBrush(std::move(brush)), mFactory(factory), mColor(c), mId(id)
    {
    }

    void D2DPen::setStrokeWidth(int w) { mStrokeWidth = w; }

    void D2DPen::setColor(const Color& c)
    {
        mColor = c;
        mBrush->SetColor(convert_to_DColorF(c));
    }

    void D2DPen::setMiterLimit(float limit)
    {
        if (limit < 1.f)
        {
            return;
        }
        mMiterLimit = limit;
        mStrokeStyle = nullptr;
    }

    void D2DPen::setDashOffset(float dashOffset)
    {
        if (dashOffset < 0.f || fuzzyCompareEqual < float, 2 > (mDashOffset, dashOffset))
        {
            return;
        }
        mDashOffset = dashOffset;
        mStrokeStyle = nullptr;
    }

    void D2DPen::setLineStyle(LineStyle style)
    {
        if (style == mLineStyle)
        {
            return;
        }
        mLineStyle = style;
        mStrokeStyle = nullptr;
    }

    void D2DPen::setLineJoin(LineJoinStyle lineJoin)
    {
        if (lineJoin == mJoinStyle)
        {
            return;
        }
        mJoinStyle = lineJoin;
        mStrokeStyle = nullptr;
    }

    void D2DPen::setLineCap(CapStyle start, CapStyle end, CapStyle dash)
    {
        setStartCap(start);
        setEndCap(end);
        setDashCap(dash);
    }

    void D2DPen::setStartCap(CapStyle start)
    {
        if (start == mStartCap)
        {
            return;
        }
        mStartCap = start;
        mStrokeStyle = nullptr;
    }

    void D2DPen::setEndCap(CapStyle end)
    {
        if (end == mEndCap)
        {
            return;
        }
        mEndCap = end;
        mStrokeStyle = nullptr;
    }

    void D2DPen::setDashCap(CapStyle dash)
    {
        if (dash == mDashCap)
        {
            return;
        }
        mDashCap = dash;
        mStrokeStyle = nullptr;
    }

    void D2DPen::setCustomDash(const vector<float>& dashes)
    {
        mDashes = dashes;
        mLineStyle = LineStyle::CustomDash;
        mStrokeStyle = nullptr;
    }

    bool D2DPen::handle(UnsafeHandle& p) noexcept
    {
        if (p.mUniqueId != mId)
        {
            return false;
        }
        if (!mStrokeStyle && shouldCreate())
        {
            createStrokeStyle();
        }
        p.set(this);
        return true;
    }

    int D2DPen::strokeWidth() const noexcept { return mStrokeWidth; }

    const Color& D2DPen::color() const noexcept { return mColor; }

    ID2D1Brush* D2DPen::brush() const noexcept { return mBrush.get(); }

    ID2D1StrokeStyle* D2DPen::strokeStyle() const noexcept { return mStrokeStyle.get(); }

    void D2DPen::createStrokeStyle()
    {
        ID2D1StrokeStyle* stroke;

        D2D1_STROKE_STYLE_PROPERTIES strokeStyleProperties =
            D2D1::StrokeStyleProperties(convert_toDCapStyle(mStartCap), // The start cap.
                                        convert_toDCapStyle(mEndCap), // The end cap.
                                        convert_toDCapStyle(mDashCap), // The dash cap.
                                        convert_toLineJoinStyle(mJoinStyle), mMiterLimit,
                                        convert_toDDashStyle(mLineStyle), // The dash style.
                                        mDashOffset // The dash offset.
            );
        float* dashes = nullptr;
        UINT dashesCount = 0;

        if (mLineStyle == LineStyle::CustomDash)
        {
            dashes = mDashes.data();
            dashesCount = numeric_cast<UINT>(mDashes.size());
        }

        if (auto hr = mFactory->CreateStrokeStyle(strokeStyleProperties, dashes, dashesCount, &stroke); hr != S_OK)
        {
            return;
        }
        mStrokeStyle = DStrokeStylePtr(stroke);
    }

    bool D2DPen::shouldCreate() const
    {
        if (mLineStyle != LineStyle::Solid)
        {
            return true;
        }
        if (mStartCap != CapStyle::Flat || mEndCap != CapStyle::Flat || mDashCap != CapStyle::Flat)
        {
            return true;
        }
        if (mJoinStyle != LineJoinStyle::Miter)
        {
            return true;
        }
        if (!fuzzyCompareEqual(mMiterLimit, 10.f) || !fuzzyCompareEqual(mDashOffset, 0.f))
        {
            return true;
        }
        return false;
    }
} // namespace bix