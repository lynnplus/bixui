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


#include "bixlib/geometry/point.h"
#include "bixlib/geometry/size.h"

namespace bix {
template <Arithmetic T>
class BIX_PUBLIC Rect {
public:
    T x1, y1, x2, y2; //left top right bottom

    Rect() {
        x1 = y1 = 0;
        x2 = y2 = -1;
    }

    Rect(T l, T t, T r, T b) {
        x1 = l;
        y1 = t;
        x2 = r;
        y2 = b;
    }

    Rect(T l, T t, Size<T> size) {
        x1 = l;
        y1 = t;
        x2 = l + size.width;
        y2 = t + size.height;
    }

    [[nodiscard]] T width() const noexcept { return x2 - x1; }
    [[nodiscard]] T height() const noexcept { return y2 - y1; }
    [[nodiscard]] T left() const noexcept { return x1; }
    [[nodiscard]] T top() const noexcept { return y1; }
    [[nodiscard]] T right() const noexcept { return x2; }
    [[nodiscard]] T bottom() const noexcept { return y2; }

    [[nodiscard]] Point<T> lt() const noexcept { return {x1, y1}; }
    [[nodiscard]] Point<T> rb() const noexcept { return {x2, y2}; }
    [[nodiscard]] Point<T> center() const { return Point((x1 + x2) / 2, (y1 + y2) / 2); }
    [[nodiscard]] Size<T> size() const { return Size(width(), height()); }

    template <typename =std::is_integral<T>>
    [[nodiscard]] bool isEmpty() const noexcept { return width() <= 0 || height() <= 0; }

    template <typename =std::is_integral<T>>
    [[nodiscard]]
    bool isValid() const { return x1 <= x2 && y1 <= y2; }

    void clear() {
        x1 = y1 = x2 = y2 = 0;
    }

    [[nodiscard]]
    Rect<int> aligned() const {
        int xmin = numeric_cast<int>(floor(x1));
        int xmax = numeric_cast<int>(ceil(x2));
        int ymin = numeric_cast<int>(floor(y1));
        int ymax = numeric_cast<int>(ceil(y2));
        return {xmin, ymin, xmax, ymax};
    }

    [[nodiscard]]
    Rect<float> centerScale(float factor) const {
        auto dw = numeric_cast<float>(x2 - x1) * factor / 2.f;
        auto dh = numeric_cast<float>(y2 - y1) * factor / 2.f;
        auto cx = numeric_cast<float>(x1 + x2) / 2.f;
        auto cy = numeric_cast<float>(y1 + y2) / 2.f;
        return {cx - dw, cy - dh, cx + dw, cy + dh};
    }

    [[nodiscard]]
    Rect<float> floated() const {
        return {
            numeric_cast<float>(x1), numeric_cast<float>(y1),
            numeric_cast<float>(x2), numeric_cast<float>(y2)
        };
    }

    Rect& setSize(const Size<T>& v) {
        x2 = x1 + v.width;
        y2 = y1 + v.height;
        return *this;
    }

    Rect& setSize(T width_, T height_) {
        x2 = x1 + width_;
        y2 = y1 + height_;
        return *this;
    }

    Rect& moveCenterTo(const Point<T>& p) noexcept {
        auto w = x2 - x1;
        auto h = y2 - y1;
        x1 = p.x - w / 2;
        y1 = p.y - h / 2;
        x2 = x1 + w;
        y2 = y1 + h;
        return *this;
    }

    Rect& moveTo(const Point<T>& p) noexcept {
        x2 += p.x - x1;
        y2 += p.y - y1;
        x1 = p.x;
        y1 = p.y;
        return *this;
    }

    Rect<float> operator *(float v) const noexcept {
        return {
            numeric_cast<float>(x1) * v, numeric_cast<float>(y1) * v, numeric_cast<float>(x2) * v,
            numeric_cast<float>(y2) * v
        };
    }

    //translate
    constexpr Rect operator+(const Point<T>& p) const noexcept {
        return {x1 + p.x, y1 + p.y, x2 + p.x, y2 + p.y};
    }

    //translate
    constexpr Rect& operator+=(const Point<T>& p) noexcept {
        x1 += p.x;
        y1 += p.y;
        x2 += p.x;
        y2 += p.y;
        return *this;
    }

    constexpr Rect operator +(const Rect& rhs) const noexcept {
        return {x1 - rhs.x1, y1 - rhs.y1, x2 + rhs.x2, y2 + rhs.y2};
    }

    constexpr Rect operator-(const Rect& rhs) const noexcept {
        return {x1 + rhs.x1, y1 + rhs.y1, x2 - rhs.x2, y2 - rhs.y2};
    }

    constexpr Rect operator +(const Size<T>& rhs) const noexcept {
        return {x1, y1, x2 + rhs.width, y2 + rhs.height};
    }

    constexpr Rect& operator+=(const Size<T>& s) noexcept {
        x2 += s.width;
        y2 += s.height;
        return *this;
    }

    constexpr bool operator ==(const Rect& rhs) const noexcept {
        if constexpr (std::is_integral_v < T >) {
            return x1 == rhs.x1 && y1 == rhs.y1 && x2 == rhs.x2 && y2 == rhs.y2;
        } else {
            return fuzzyCompareEqual(x1, rhs.x1) && fuzzyCompareEqual(y1, rhs.y1) &&
                fuzzyCompareEqual(x2, rhs.x2) && fuzzyCompareEqual(y2, rhs.y2);
        }
    }
};

using UIRect = Rect<int>;
using RectF = Rect<float>;
}