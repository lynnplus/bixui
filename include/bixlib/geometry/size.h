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

#include "bixlib/common.h"
#include "bixlib/export_macro.h"
#include "bixlib/utils/numeric.h"

namespace bix {
template <Arithmetic T>
class BIX_PUBLIC Size {
public:
    T width, height;
    Size() : width(-1), height(-1) {}
    Size(T width_, T height_) : width(width_), height(height_) {}

    Size<float> scale(float factor) const noexcept {
        return {numeric_cast<float>(width) * factor, numeric_cast<float>(height) * factor};
    }

    Size<int> aligned() const noexcept { return {numeric_cast<int>(ceil(width)), numeric_cast<int>(ceil(height))}; }

    bool isValid() const noexcept { return width >= 0 && height >= 0; }

    template <typename = std::is_integral<T>>
    bool isEmpty() const noexcept {
        return width <= 0 || height <= 0;
    }

    Size<float> operator/(float v) const noexcept {
        return {numeric_cast<float>(width) / v, numeric_cast<float>(height) / v};
    }

    Size<float> operator*(float v) const noexcept {
        return {numeric_cast<float>(width) * v, numeric_cast<float>(height) * v};
    }

    constexpr bool operator==(Size const& rhs) noexcept { return comparesEqual(rhs); }
    constexpr bool operator!=(Size const& rhs) noexcept { return !comparesEqual(rhs); }

private:
    constexpr bool comparesEqual(const Size& rhs) noexcept { return width == rhs.width && height == rhs.height; }
};

using UISize = Size<int>;
using SizeF = Size<float>;
} // namespace bix
