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
#include "bixlib/render/brush.h"
#include "bixlib/render/pen.h"
#include "bixlib/render/text_format.h"
#include "bixlib/render/transform.h"

namespace bix {

struct ClipRoundRect {
    UIRect rect;
    int radius;
};

struct TextMetrics {
    int minWidth;
    int width;
    int height;
    int lineCount;
};

class BIX_PUBLIC Canvas {
public:
    virtual ~Canvas() = default;

    virtual SizeF size() const noexcept = 0;

    virtual void beginDraw() = 0;
    virtual void endDraw() = 0;

    virtual void resize(const SizeF& size) = 0;
    virtual void clear(const Color& c) = 0;
    virtual void setTransform(const Transform& transform) = 0;

    [[nodiscard]]
    virtual ColorBrushPtr createColorBrush(const Color& color) = 0;
    [[nodiscard]]
    virtual PenPtr createPen(const Color& color) = 0;
    [[nodiscard]]
    virtual TextPaintPtr createTextPaint() = 0;

    virtual void pushClip(const UIRoundRect& rect) = 0;
    virtual void popClip() = 0;

    virtual void fillRectangle(const UIRect& rect, Brush& brush) = 0;

    /**
     * Draws a rectangle outline on the canvas using the specified pen.
     *
     * This function draws a rectangle outline on the canvas based on the provided Rect object.
     * The outline is composed of four straight lines, and its appearance is determined by the Pen object.
     *
     * @param[in] rect The Rect object defining the rectangle's position and size.
     * @param[in] pen The Pen object specifying the line style, color, and width of the outline.
     *
     * @note The rectangular outline is drawn along the coordinates provided by Rect,
     * extending from the edge inwards by the width (Pen::strokeWidth()) of the Pen,
     * occupying the inner area.
     * @warning If the Rect object is invalid (e.g., negative dimensions), the behavior is undefined.
     * If the Pen's width is 0, the rectangle will not be drawn.
     */
    virtual void drawRectangle(const UIRect& rect, Pen& pen) = 0;

    virtual void drawRoundRect(const UIRect& rect, int radiusX, int radiusY, Pen& pen) = 0;
    virtual void drawEllipse(const UIEllipse& ellipse, Pen& pen) = 0;

    virtual void measureText(TextPaint& format, TextMetrics& metrics) = 0;
    virtual void drawText(const UIPoint& origin, TextPaint& text, Brush& brush) = 0;
    virtual void drawLine(const UIPoint& p0, const UIPoint& p1, Pen& pen) = 0;
    virtual void drawLines(const std::vector<UILine>& lines, Pen& pen) = 0;

    // void drawPolyline();
    // void drawPolygon();
    // void drawConvexPolygon();
    // void drawArc();
    // void drawPie();
    // void drawRoundedRect();
};

using CanvasPtr = std::unique_ptr<Canvas>;
} // namespace bix
