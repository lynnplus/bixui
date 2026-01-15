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

#include "bixlib/geometry.h"
#include "bixlib/graphics/color.h"
#include "bixlib/graphics/transform.h"

#include <d2d1.h>

#include <cassert>

namespace bix {

inline D2D1_POINT_2F convert_to_DPointF(const UIPoint& src) {
    return {numeric_cast<float>(src.x), numeric_cast<float>(src.y)};
}

inline D2D1_RECT_F convert_to_DRectF(const Rect<int>& src) {
    return {
        numeric_cast<float>(src.x1),
        numeric_cast<float>(src.y1),
        numeric_cast<float>(src.x2),
        numeric_cast<float>(src.y2)
    };
}

inline D2D1_RECT_F convert_to_DRectF(const Rect<float>& src) {
    return {src.x1, src.y1, src.x2, src.y2};
}

inline D2D1_ROUNDED_RECT convert_to_DRoundRect(const Rect<int>& src, int radiusX, int radiusY) {
    return {convert_to_DRectF(src), numeric_cast<float>(radiusX), numeric_cast<float>(radiusY)};
}

inline D2D1_ELLIPSE convert_to_Ellipse(const Ellipse<int>& src) {
    return {convert_to_DPointF(src.point), numeric_cast<float>(src.radiusX), numeric_cast<float>(src.radiusY)};
}

inline D2D1_COLOR_F convert_to_DColorF(const Color& src) {
    assert(src.isValid());

    auto to = [](int v) { return static_cast<float>(v) / 255.f; };
    return {to(src.red()), to(src.green()), to(src.blue()), to(src.alpha())};
}

inline Color convert_from_DColor(const D2D1_COLOR_F& src) {
    auto to = [](float v) { return static_cast<int>(v * 255.f); };
    return {to(src.r), to(src.g), to(src.b), to(src.a)};
}

inline D2D1_MATRIX_3X2_F convert_to_DMatrix(const Transform& transform) {
    auto data = transform.data();
    return D2D1::Matrix3x2F(data[0], data[1], data[3], data[4], data[6], data[7]);
}

} // namespace bix
