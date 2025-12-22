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
#include "bixlib/geometry/shape.h"
#include "bixlib/geometry/size.h"

namespace bix {

template <Arithmetic T>
class Margins;

template <Arithmetic T>
class BIX_PUBLIC Rect {
public:
    T x1, y1, x2, y2; // left top right bottom

    Rect() {
        x1 = y1 = 0;
        x2 = y2 = -1;
    }

    explicit Rect(const Size<T>& size) : Rect(0, 0, size) {}

    Rect(T l, T t, T r, T b) {
        x1 = l;
        y1 = t;
        x2 = r;
        y2 = b;
    }

    Rect(T l, T t, const Size<T>& size) {
        x1 = l;
        y1 = t;
        x2 = l + size.width;
        y2 = t + size.height;
    }

    T width() const noexcept { return x2 - x1; }

    T height() const noexcept { return y2 - y1; }

    T left() const noexcept { return x1; }

    T top() const noexcept { return y1; }

    T right() const noexcept { return x2; }

    T bottom() const noexcept { return y2; }

    Point<T> lt() const noexcept { return {x1, y1}; }

    Point<T> rb() const noexcept { return {x2, y2}; }

    Point<T> center() const { return Point((x1 + x2) / 2, (y1 + y2) / 2); }

    Size<T> size() const { return Size(width(), height()); }

    Size<T> asPadding() const noexcept { return {width(), height()}; }

    bool isEmpty() const noexcept { return width() <= 0 || height() <= 0; }

    bool isValid() const { return x1 <= x2 && y1 <= y2; }

    void clear() { x1 = y1 = x2 = y2 = 0; }

    Rect<int> aligned() const {
        int xmin = numeric_cast<int>(std::floor(x1));
        int xmax = numeric_cast<int>(std::ceil(x2));
        int ymin = numeric_cast<int>(std::floor(y1));
        int ymax = numeric_cast<int>(std::ceil(y2));
        return {xmin, ymin, xmax, ymax};
    }

    Rect<float> centerScale(float factor) const {
        auto dw = numeric_cast<float>(x2 - x1) * factor / 2.f;
        auto dh = numeric_cast<float>(y2 - y1) * factor / 2.f;
        auto cx = numeric_cast<float>(x1 + x2) / 2.f;
        auto cy = numeric_cast<float>(y1 + y2) / 2.f;
        return {cx - dw, cy - dh, cx + dw, cy + dh};
    }

    Rect<float> floated() const {
        return {numeric_cast<float>(x1), numeric_cast<float>(y1), numeric_cast<float>(x2), numeric_cast<float>(y2)};
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

    Rect& setWidth(T width_) {
        x2 = x1 + width_;
        return *this;
    }

    Rect& setHeight(T height_) {
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

    Rect<float> operator*(float v) const noexcept {
        return {numeric_cast<float>(x1) * v, numeric_cast<float>(y1) * v, numeric_cast<float>(x2) * v,
                numeric_cast<float>(y2) * v};
    }

    // translate
    constexpr Rect operator+(const Point<T>& p) const noexcept { return {x1 + p.x, y1 + p.y, x2 + p.x, y2 + p.y}; }

    // translate
    constexpr Rect& operator+=(const Point<T>& p) noexcept {
        x1 += p.x;
        y1 += p.y;
        x2 += p.x;
        y2 += p.y;
        return *this;
    }

    constexpr Rect removeMargin(T margin) const noexcept {
        return {x1 + margin, y1 + margin, x2 - margin, y2 - margin};
    }

    constexpr Rect operator-(const Margins<T>& margins) const noexcept {
        return {x1 + margins.left, y1 + margins.top, x2 - margins.right, y2 - margins.bottom};
    }

    constexpr Rect operator+(const Size<T>& rhs) const noexcept { return {x1, y1, x2 + rhs.width, y2 + rhs.height}; }

    constexpr Rect& operator+=(const Size<T>& s) noexcept {
        x2 += s.width;
        y2 += s.height;
        return *this;
    }

    constexpr bool operator==(const Rect& rhs) const noexcept {
        if constexpr (std::is_integral_v<T>) {
            return x1 == rhs.x1 && y1 == rhs.y1 && x2 == rhs.x2 && y2 == rhs.y2;
        } else {
            return fuzzyCompareEqual(x1, rhs.x1) && fuzzyCompareEqual(y1, rhs.y1) && fuzzyCompareEqual(x2, rhs.x2) &&
                   fuzzyCompareEqual(y2, rhs.y2);
        }
    }

    bool contains(const Point<T> pt) const noexcept {
        if (isEmpty()) {
            return false;
        }
        if (pt.x < left() || pt.x > right() || pt.y < top() || pt.y > bottom()) {
            return false;
        }
        return true;
    }
};

using UIRect = Rect<int>;
using RectF = Rect<float>;

/**
 * Quarter-ellipse radius template
 *
 * Represents corner radii for rounded rectangles, supports uniform setting and validation,
 * Each corner is modeled as a quarter-ellipse,
 * Radius values are specified by #radiusX (horizontal) and #radiusY (vertical),
 * Template type T defines the underlying data type (e.g., int, float)
 *
 * @tparam T Arithmetic type (integral or floating-point)
 */
template <Arithmetic T>
class CornerRadius {
public:
    T radiusX = 0; ///< Horizontal radius
    T radiusY = 0; ///< Vertical radius

    CornerRadius() = default;

    explicit CornerRadius(T radius) : CornerRadius(radius, radius) {}

    CornerRadius(T rx, T ry) : radiusX(rx), radiusY(ry) {}

    /**
     * Set radius
     * @param[in] radius New radius value
     * @return Reference to current object
     */
    CornerRadius& setRadius(T radius) noexcept {
        radiusX = radiusX = radius;
        return *this;
    }

    CornerRadius& setRadius(T rx, T ry) noexcept {
        radiusX = rx;
        radiusY = ry;
        return *this;
    }

    /**
     * Check if radii are empty
     * @return True if both radii are zero
     * @note Direct comparison for integers, fuzzy comparison for floats
     */
    constexpr bool isEmpty() const noexcept {
        if constexpr (std::is_integral_v<T>) {
            return radiusX == 0 && radiusY == 0;
        } else {
            return fuzzyEqualZero(radiusX) && fuzzyEqualZero(radiusY);
        }
    }

    /**
     * Validate radii
     * @return True if both radii are non-negative
     */
    constexpr bool isValid() const noexcept { return radiusX >= 0 && radiusY >= 0; }

    constexpr bool operator==(const CornerRadius& rhs) const noexcept {
        if constexpr (std::is_integral_v<T>) {
            return radiusX == rhs.radiusX && radiusY == rhs.radiusY;
        } else {
            return fuzzyCompareEqual(radiusX, rhs.radiusX) && fuzzyCompareEqual(radiusY, rhs.radiusY);
        }
    }
};

template <Arithmetic T>
class BIX_PUBLIC RoundedRect : public Rect<T> {
public:
    CornerRadius<T> radii;

    /**
     * Constructor for RoundedRect.
     * Initialize a rounded rectangle with uniform corner radius
     * @param size Size of the rectangle
     * @param radius Uniform radius for all corners
     */
    RoundedRect(const Size<T>& size, T radius) : Rect<T>(size), radii(radius) {}

    void setRadius(T radius_) noexcept { radii = {radius_}; }
};

using UIRoundRect = RoundedRect<int>;

// Flexible Rectangle
template <Arithmetic T>
class FlexRoundedRect : public Rect<T> {
public:
    CornerRadius<T> tl, tr, bl, br;

    void setRadius(CornerRadius<T> radius) noexcept {
        tl = radius;
        tr = radius;
        bl = radius;
        br = radius;
    }

    [[nodiscard]]
    ShapeType shape() const {
        if (tl == tr && tl == bl && tl == br) {
            if (tl.isEmpty()) {
                return ShapeType::Rectangle;
            }
            if (tl.radiusX >= this->width() / 2 && tl.radiusY >= this->height() / 2) {
                // return ShapeType::Ellipse;
            }
            return ShapeType::RoundedRectangle;
        }
        return ShapeType::FlexRoundedRectangle;
    }
};

using UIFlexRoundedRect = FlexRoundedRect<int>;

template <Arithmetic T>
class BIX_PUBLIC Margins {
public:
    T left = 0, top = 0, right = 0, bottom = 0;

    Margins() = default;

    explicit Margins(T value) : Margins(value, value, value, value) {}

    Margins(T leftRight, T topBottom) : Margins(leftRight, topBottom, leftRight, topBottom) {}

    Margins(T left, T top, T right, T bottom) : left(left), top(top), right(right), bottom(bottom) {}

    T totalX() const noexcept { return left + right; }

    T totalY() const noexcept { return top + bottom; }

    Margins operator+(T v) const noexcept { return {left + v, top + v, right, bottom + v}; }

    Margins operator+(const Margins& margins) const noexcept {
        return {left + margins.left, top + margins.top, right + margins.right, bottom + margins.bottom};
    }
};

using UIMargins = Margins<int>;
using UIPaddings = Margins<int>;

} // namespace bix