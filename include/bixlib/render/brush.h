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
#include <memory>

#include "unsafe.h"
#include "bixlib/render/color.h"

namespace bix {
enum class BrushStyle {
    SolidColor,
    LinearGradient,
    RadialGradient,
};

class BIX_PUBLIC Brush {
public:
    virtual ~Brush() = default;

    virtual void setOpacity(float opacity) =0;
    [[nodiscard]] virtual float opacity() const noexcept =0;
    [[nodiscard]] virtual BrushStyle style() const noexcept =0;
    virtual bool handle(UnsafeHandle& p) noexcept =0;
};


class BIX_PUBLIC ColorBrush : public Brush {
public:
    virtual void setColor(const Color& color) =0;
    [[nodiscard]] virtual Color color() const noexcept =0;

    [[nodiscard]] BrushStyle style() const noexcept override {
        return BrushStyle::SolidColor;
    }
};

using BrushPtr = std::unique_ptr<Brush>;
using ColorBrushPtr = std::unique_ptr<ColorBrush>;
}
