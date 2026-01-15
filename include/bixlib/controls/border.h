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
#include "../core/length.h"
#include "bixlib/geometry/shape.h"
#include "bixlib/graphics/canvas.h"
#include "bixlib/utils/flags.h"

namespace bix {

/**
 * Represents the border stroke properties of an element.
 */
class BorderStroke {
public:
    Color color = colors::White; ///< The color of the border stroke.
    int width = 0;               ///< The width of the border stroke,default value is 0, indicating no border.
    LineStyle lineStyle = LineStyle::Solid; ///< The line style of the border stroke,default is solid line
    /**
     * Indicates whether the border stroke overlays the content area.
     *
     * If true, The border stroke will overlay the content area
     * and will not take up any content area space.
     *
     * If false, the border stroke is drawn within the content area,
     * occupies the size of the content area, and behaves the same as padding.
     */
    bool overlay = false;

    bool operator==(const BorderStroke& rhs) const noexcept {
        return color == rhs.color && width == rhs.width && lineStyle == rhs.lineStyle && overlay == rhs.overlay;
    }
};

enum class BorderFlag : uint8_t {
    Dirty = 1 << 0,
    SetEllipse = 1 << 1,
    HasLeft = 1 << 2,
    HasRight = 1 << 3,
    HasTop = 1 << 4,
    HasBottom = 1 << 5,

    HasAll = 0b0011'1100, ///< left|right|top|bottom
};
BIX_DECLARE_ENUM_FLAGS(BorderFlag)

struct BorderRadius {
    Length radiusX, radiusY;
};

class BIX_PUBLIC Border {
public:
    using BorderFlags = Flags<BorderFlag>;
    virtual ~Border() = default;

    Border() = delete;
    // explicit Border(const BorderStroke& stroke, int radius = 0);
    // Border(int left, int top, int right, int bottom,const Color& color);

    // Border(const BorderStroke& left, const BorderStroke& top, const BorderStroke& right, const BorderStroke& bottom);

    Border& setStroke(const BorderStroke& stroke);
    Border& setLeft(const BorderStroke& stroke);
    Border& setTop(const BorderStroke& stroke);
    Border& setRight(const BorderStroke& stroke);
    Border& setBottom(const BorderStroke& stroke);

    Border& setRadius(const Length& radius);
    Border& setRadius(const Length& rx, const Length& ry);

    // Border& setTopLeftRadius(const Length& x, const Length& y);
    // Border& setTopRightRadius(const Length& x, const Length& y);
    // Border& setBottomLeftRadius(const Length& x, const Length& y);
    // Border& setBottomRightRadius(const Length& x, const Length& y);

    /**
     * Get the internal padding occupied by the border
     *
     * Returns the padding values for the four directions (top, right, bottom, left).
     * If the border is drawn as an overlay, it does not occupy the content area.
     * Otherwise, returns the border stroke width.
     * The returned values are included in the control's actual padding.
     * @return the UIPaddings object representing the padding
     */
    virtual UIPaddings insets() const;
    virtual UIFlexRoundedRect makeRect(const UIRect& rect) const noexcept;

    virtual void reset();
    virtual void onDraw(const UIRect& rect, Canvas& canvas);
    virtual void onDiscardCanvas();

protected:
    BorderRadius mTopLeftRadius, mTopRightRadius, mBottomLeftRadius, mBottomRightRadius;
    BorderStroke mLeft, mTop, mRight, mBottom;
    BorderFlags mFlags{BorderFlag::Dirty};
    float mEllipseRadiusX = 0, mEllipseRadiusY = 0;
    ShapeType mShapeType = ShapeType::None;
    PenPtr mLeftPen, mTopPen, mRightPen, mBottomPen;

private:
    void update();
};

using BorderPtr = std::unique_ptr<Border>;

} // namespace bix
