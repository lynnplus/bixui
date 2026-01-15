/*
 * Copyright (c) 2025-2026 Lynn <lynnplus90@gmail.com>
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
#include <bixlib/core/length.h>

namespace bix {

struct BIX_PUBLIC LayoutSize {
    Length width;
    Length height;

    constexpr LayoutSize() noexcept = default;

    constexpr LayoutSize(Length w, Length h) noexcept : width(w), height(h) {}

    static constexpr LayoutSize autoSize() noexcept { return {Length::autoSize(), Length::autoSize()}; }

    bool isFixed() const noexcept { return width.unit() == Length::PX && height.unit() == Length::PX; }
};

struct BIX_PUBLIC BoxConstraints {
    Length minWidth = Length::px(0);
    Length minHeight = Length::px(0);
    Length maxWidth = Length::infinity();
    Length maxHeight = Length::infinity();
};

} // namespace bix