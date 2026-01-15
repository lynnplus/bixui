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

#include "../window/window.h"
#include "bixlib/graphics/canvas.h"

namespace bix {
class BIX_PUBLIC RenderEngine {
public:
    virtual ~RenderEngine() = default;

    enum Type {
        Direct2D,
        GdiPlus,
        X11,

        UserCustom = 100
    };

    static RenderEngine* from(Type t);

    virtual void shutdown() noexcept = 0;

    virtual Type type() const noexcept = 0;

    virtual CanvasPtr createCanvas(const Window& w) = 0;
};
} // namespace bix