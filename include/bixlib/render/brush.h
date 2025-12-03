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
#include "bixlib/render/unsafe.h"

#include <memory>

namespace bix {
enum class BrushStyle {
    SolidColor,
    LinearGradient,
    RadialGradient,
};

/**
 * Abstract base class for brush objects used in graphics rendering
 *
 * This class defines the interface for brushes that can be used for
 * filling shapes, drawing operations, and other graphics primitives.
 */
class BIX_PUBLIC Brush {
public:
    virtual ~Brush() = default;

    /**
     * Set the opacity level of the brush
     * @param opacity Opacity value in range [0.0, 1.0], where 0.0 is fully transparent and 1.0 is fully opaque
     */
    virtual void setOpacity(float opacity) = 0;
    /**
     * Get the current opacity level of the brush
     * @return Current opacity value in range [0.0, 1.0]
     */
    virtual float opacity() const noexcept = 0;
    /**
     * Get the style of the brush
     * @return The BrushStyle enum value representing the brush's style
     */
    virtual BrushStyle style() const noexcept = 0;
    virtual bool handle(UnsafeHandle& p) noexcept = 0;
};

using BrushPtr = std::unique_ptr<Brush>;

class BIX_PUBLIC ColorBrush : public Brush {
public:
    virtual void setColor(const Color& color) = 0;
    virtual Color color() const noexcept = 0;

    BrushStyle style() const noexcept override { return BrushStyle::SolidColor; }
};

using ColorBrushPtr = std::unique_ptr<ColorBrush>;

class BIX_PUBLIC LinearGradientBrush : public Brush {
public:
};
} // namespace bix
