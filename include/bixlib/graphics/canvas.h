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
#include "bixlib/graphics/brush.h"
#include "bixlib/graphics/pen.h"
#include "bixlib/graphics/text_format.h"
#include "bixlib/graphics/transform.h"

namespace bix {

struct TextMetrics {
    int minWidth;
    int width;
    int height;
    int lineCount;
};

enum class DrawResult {
    Success,
    Error,
    RecreateCanvas
};

/**
 * @class Canvas
 * @brief interface base class for rendering graphics on a 2D surface.
 *
 * The Canvas class provides a virtual interface for drawing operations on a 2D canvas.
 * It defines a set of pure virtual methods that must be implemented by derived classes
 * to support specific rendering backends (e.g., software rendering, GPU acceleration).
 *
 * Key Features:
 * - Supports basic drawing primitives: rectangles, ellipses, lines, and text.
 * - Manages drawing state through transformation matrices and clipping regions.
 * - Provides resource creation methods for brushes, pens, and text paints.
 * - Abstracts platform-specific rendering details, enabling cross-platform compatibility.
 *
 * Usage:
 * 1. Implement derived classes for specific rendering engines (e.g., OpenGLCanvas, SkiaCanvas).
 * 2. Use Canvas methods to perform drawing operations in a device-independent manner.
 * 3. Manage canvas lifecycle with beginDraw() and endDraw() for efficient rendering.
 *
 * @note When the drawing operation unit is not explicitly stated, the default operation unit is px.
 *
 * @note This class is part of the BIX graphics library and is intended for internal use.
 * External developers should interact with Canvas through its public interface.
 */
class BIX_PUBLIC Canvas {
public:
    virtual ~Canvas() = default;

    /**
     * Retrieves the current size of the canvas.
     * @return The size of the canvas as a SizeF object.
     */
    virtual Size size() const noexcept = 0;
    /**
     * Begins the drawing process on the canvas.
     * This method should be called before any drawing operations.
     */
    virtual void beginDraw() = 0;
    /**
     * Ends the drawing process on the canvas.
     * This method should be called after all drawing operations are completed.
     */
    virtual DrawResult endDraw() = 0;
    /**
     * Resizes the canvas to the specified dimensions.
     * @param[in] size The new size for the canvas.
     */
    virtual void resize(const Size& size) = 0;
    /**
     * Clears the canvas with the specified color.
     * @param[in] c The color used to clear the canvas.
     */
    virtual void clear(const Color& c) = 0;
    /**
     * Sets the transformation matrix for the canvas.
     * @param[in] transform The transformation matrix to apply.
     */
    virtual void setTransform(const Transform& transform) = 0;

    /**
     * Creates a color brush for drawing operations.
     * @param[in] color The color of the brush.
     * @return A unique pointer to the created ColorBrush.
     */
    [[nodiscard]]
    virtual ColorBrushPtr createColorBrush(const Color& color) = 0;
    /**
     * Creates a pen for drawing operations.
     * @param[in] color The color of the pen.
     * @return A unique pointer to the created Pen.
     */
    [[nodiscard]]
    virtual PenPtr createPen(const Color& color) = 0;
    /**
     * Creates a text paint object for text rendering.
     * @return A unique pointer to the created TextPaint.
     */
    [[nodiscard]]
    virtual TextPaintPtr createTextPaint() = 0;

    /**
     * Pushes a clipping region onto the canvas.
     * @param[in] rect The rectangular region to clip.
     * @return True if the clipping region was successfully pushed, false otherwise.
     */
    virtual bool pushClip(const UIFlexRoundedRect& rect) = 0;
    /**
     * Pops the top clipping region from the canvas.
     */
    virtual void popClip() = 0;
    /**
     * Fills a rectangle on the canvas with a brush.
     * @param[in] rect The rectangle to fill.
     * @param[in] brush The brush used for filling.
     */
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
     */
    virtual void drawRectangle(const UIRect& rect, Pen& pen) = 0;
    /**
     * Draws a rounded rectangle outline on the canvas.
     * @param[in] rect The rectangle to draw.
     * @param[in] radiusX The horizontal radius of the rounded corners.
     * @param[in] radiusY The vertical radius of the rounded corners.
     * @param[in] pen The pen used for drawing the outline.
     */
    virtual void drawRoundRect(const UIRect& rect, int radiusX, int radiusY, Pen& pen) = 0;
    /**
     * Draws an ellipse outline on the canvas.
     * @param[in] ellipse The ellipse to draw.
     * @param[in] pen The pen used for drawing the outline.
     */
    virtual void drawEllipse(const UIEllipse& ellipse, Pen& pen) = 0;
    /**
     * Measures the metrics of a text string.
     * @param[in] format The text paint object.
     * @param[out] metrics The text metrics object to store the results.
     */
    virtual void measureText(TextPaint& format, TextMetrics& metrics) = 0;
    /**
     * Draws text on the canvas.
     * @param[in] origin The starting point for the text.
     * @param[in] text The text paint object.
     * @param[in] pen The pen used for text rendering.
     */
    virtual void drawText(const UIPoint& origin, TextPaint& text, Pen& pen) = 0;
    virtual void drawLine(const UILine& line, Pen& pen) = 0;
    /**
     * Draws multiple lines on the canvas.
     * @param[in] lines The vector of lines to draw.
     * @param[in] pen The pen used for drawing the lines.
     */
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
