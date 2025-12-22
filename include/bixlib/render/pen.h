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

#include "bixlib/render/color.h"

#include <vector>

namespace bix {
enum class LineStyle {
    Solid,
    Dash,
    Dot,
    DashDot,
    DashDotDot,
    CustomDash
};

enum class CapStyle {
    Flat,
    Square,
    Round,
    Triangle,
};

enum class LineJoinStyle {
    Miter = 0,
    Bevel = 1,
    Round = 2,
    MiterOrBevel = 3
};

class BIX_PUBLIC Pen {
public:
    virtual ~Pen() = default;
    virtual void setColor(const Color& c) = 0;
    virtual void setStrokeWidth(int w) = 0; // unit px
    virtual void setLineStyle(LineStyle style) = 0;
    virtual void setLineJoin(LineJoinStyle lineJoin) = 0;
    virtual void setLineCap(CapStyle start, CapStyle end, CapStyle dash) = 0;
    virtual void setStartCap(CapStyle start) = 0;
    virtual void setEndCap(CapStyle end) = 0;
    virtual void setDashCap(CapStyle dash) = 0;
    virtual void setMiterLimit(float limit) = 0;
    virtual void setDashOffset(float dashOffset) = 0;
    /**
     * Set the floating point array of custom dash. Using this method, LineStyle will be automatically set to
     * CustomDash.
     * @param dashes
     */
    virtual void setCustomDash(const std::vector<float>& dashes) = 0;

    virtual const Color& color() const noexcept = 0;

    /**
     * get the stroke width of the pen.
     *
     * @return The stroke width of the pen.
     */
    virtual int strokeWidth() const noexcept = 0;

    virtual bool testCast(uintptr_t scope, long castId) const noexcept = 0;
};

/**
 * std::unique_ptr<Pen> alias for Pen objects
 */
using PenPtr = std::unique_ptr<Pen>;
} // namespace bix
