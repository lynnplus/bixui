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

#include <d2d1.h>
#include <dwrite.h>
#include <memory>

namespace bix {

struct IUnknownDeleter {
    void operator()(IUnknown* b) const {
        if (b) {
            b->Release();
        }
    }
};

using DFactorPtr = std::unique_ptr<ID2D1Factory, IUnknownDeleter>;
using DWriteFactoryPtr = std::unique_ptr<IDWriteFactory, IUnknownDeleter>;
using DBrushPtr = std::unique_ptr<ID2D1Brush, IUnknownDeleter>;
using DSolidColorBrushPtr = std::unique_ptr<ID2D1SolidColorBrush, IUnknownDeleter>;
using DHwndRenderTargetPtr = std::unique_ptr<ID2D1HwndRenderTarget, IUnknownDeleter>;
using DWriteTextFormatPtr = std::unique_ptr<IDWriteTextFormat, IUnknownDeleter>;
using DWriteTextLayoutPtr = std::unique_ptr<IDWriteTextLayout, IUnknownDeleter>;
using DStrokeStylePtr = std::unique_ptr<ID2D1StrokeStyle, IUnknownDeleter>;
using DLayerPtr = std::unique_ptr<ID2D1Layer, IUnknownDeleter>;
using DWInlineObjPtr = std::unique_ptr<IDWriteInlineObject, IUnknownDeleter>;

} // namespace bix
