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

#include "brush.h"

#include "convert-inl.h"

namespace bix {

D2DSolidColorBrush::D2DSolidColorBrush(DSolidColorBrushPtr b, ID2D1RenderTarget* rt, uintptr_t id)
    : D2DBasicBrush(rt, id), mBrushImpl(std::move(b)) {}

void D2DSolidColorBrush::setColor(const Color& color) { mBrushImpl->SetColor(convert_to_DColorF(color)); }

Color D2DSolidColorBrush::color() const noexcept {
    // TODO not implemented
    return {};
}

ID2D1Brush* D2DSolidColorBrush::current() const { return mBrushImpl.get(); }
} // namespace bix
