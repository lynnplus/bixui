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

#include "bixlib/render/engine.h"
#include "direct2d.h"

namespace bix {

class Direct2DEngine : public RenderEngine {
public:
    static Direct2DEngine* instance() {
        static Direct2DEngine instance;
        return &instance;
    }

    ~Direct2DEngine() override;

    void shutdown() noexcept override;
    Type type() const noexcept override;

    [[nodiscard]]
    CanvasPtr createCanvas(const Window& w) override;

    IDWriteTextFormat* createFont();

protected:
    DFactorPtr mD2DFactory = nullptr;
    DWriteFactoryPtr mDWriteFactory = nullptr;

private:
    Direct2DEngine();
};

} // namespace bix
