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

#include "bixlib/export_macro.h"
#include "bixlib/utils/concepts.h"
#include "bixlib/utils/numeric.h"

namespace bix {
template <Arithmetic T>
struct BIX_PUBLIC Point {
    T x, y;

    Point() : x(0), y(0) {}

    Point(T x_, T y_) : x(x_), y(y_) {}

    Point<float> operator/(float density) const noexcept {
        Point<float> result{numeric_cast<float>(x), numeric_cast<float>(y)};
        if (fuzzyEqualZero<float, 4>(density)) {
            return result;
        }
        result.x /= density;
        result.y /= density;
        return result;
    }

    Point<int> floor() const noexcept {
        return Point<int>(numeric_cast<int>(std::floor(x)), numeric_cast<int>(std::floor(y)));
    }
};

template <Arithmetic T>
struct BIX_PUBLIC LineSegment {
    Point<T> p0, p1;
};

using UIPoint = Point<int>;
using UILine = LineSegment<int>;
} // namespace bix