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

#include "brush.h"
#include "pen.h"
#include "text_format.h"
#include "transform.h"

namespace bix {
class BIX_PUBLIC Canvas {
public:
    virtual ~Canvas() = default;

    [[nodiscard]]
    virtual SizeF size() const noexcept =0;

    virtual void beginDraw() =0;
    virtual void endDraw() =0;

    virtual void resize(const SizeF& size) =0;
    virtual void clear(const Color& c) =0;
    virtual void setTransform(const Transform& transform) =0;

    [[nodiscard]]
    virtual ColorBrushPtr createColorBrush(const Color& color) const =0;
    [[nodiscard]]
    virtual PenPtr createPen(const Color& color) const =0;

    [[nodiscard]]
    virtual TextPaintPtr createTextPaint() const =0;

    virtual void pushClip(const UIRect& rect) =0;
    virtual void popClip() =0;


    // [[nodiscard]]
    // virtual RenderTarget* target() const=0;
    // virtual void drawLine(const PointF& start, const PointF& end, const Brush& brush) =0;
    virtual void fillRectangle(const UIRect& rect, Brush& brush) =0;


    virtual void drawRectangle(const UIRect& rect, Pen& pen) =0;

    /**
     * Draws the outline of the specified rounded rectangle using the specified stroke style
     * @param pos The position of the rounded rectangle to be drawn
     * @param strokeSize The width of each outline stroke of the rounded rectangle to be drawn.
     * @param rx The x-radius for the quarter ellipse that is drawn to replace every corner of the rectangle.
     * @param ry The y-radius for the quarter ellipse that is drawn to replace every corner of the rectangle.
     * @param pen A pen used to draw the outline of a rounded rectangle
     */
    virtual void drawRoundRect(const UIRect& pos, const UIRect& strokeSize, int rx, int ry, Pen& pen) =0;
    // virtual void drawEllipse(const EllipseF& ellipse, const Brush& brush) =0;
    // virtual void drawLine(const PointF& p0, const PointF& p1) =0;
    // virtual void drawLines(const std::vector<PointF>& lines) =0;

    virtual void measureText(const std::string& text, TextPaint& format, UIRect& boundingBox) =0;
    virtual void drawText(TextPaint& text, Brush& brush) =0;

    // void drawPolyline();
    // void drawPolygon();
    // void drawConvexPolygon();
    // void drawArc();
    // void drawPie();
    // void drawRoundedRect();
};

using CanvasPtr = std::unique_ptr<Canvas>;
}
