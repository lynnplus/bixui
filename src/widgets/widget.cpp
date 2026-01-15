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

#include "bixlib/widgets/widget.h"

#include <bixlib/assert.h>

#include <algorithm>

namespace bix {

bool Widget::isEnabled() const noexcept {
    return !mFlags.testFlag(ControlFlag::Disable);
}

bool Widget::isHovered() const noexcept {
    return mIsHovered;
}

bool Widget::isClickable() const noexcept {
    return mFlags.testFlag(ControlFlag::Clickable);
}

const UIRect& Widget::position() const noexcept {
    return mPosition;
}

Border* Widget::border() const noexcept {
    return mBorder.get();
}

UIPaddings Widget::getPaddingWithForeground() const noexcept {
    if (mBorder) { return mPadding + mBorder->insets(); }
    return mPadding;
}

void Widget::invalidate() {}

void Widget::setParent(ViewParent* parent) {
    mParent = parent;
}

void Widget::setWidth(Length width) {
    if (mWidth == width) { return; }

    mWidth = width;

    requestLayout();
}

void Widget::setHeight(Length height) {
    if (mHeight == height) { return; }

    mHeight = height;
    requestLayout();
}

void Widget::setMargins(const UIMargins& margin) {
    mMargin = margin;
}

void Widget::setPadding(const UIPaddings& padding) {
    mPadding = padding;
}

// void Widget::setSize(const UILength& w, const UILength& h) {
//     mSize = {w, h};
//     // TODO
//     mMeasuredSize.width = w.fixed();
//     mMeasuredSize.height = h.fixed();
// }

void Widget::setMaximumSize(const UISize& s) {
    if (mMinSize.width > 0 && mMinSize.width > s.width) {
        // TODO throw ?
        return;
    }
    if (mMinSize.height > 0 && mMinSize.height > s.height) { return; }
    mMaxSize = s;
}

void Widget::setMinimumSize(const UISize& s) {
    if (mMaxSize.width > 0 && mMaxSize.width < s.width) { return; }
    if (mMaxSize.height > 0 && mMaxSize.height < s.height) { return; }
    mMinSize = s;
}

void Widget::setVisibility(Visibility value) {
    if (mVisibility == value) return;
    mVisibility = value;

    if (value == Visibility::Visible) {
        mFlags.unset(WidgetFlag::WillNotDraw);
        markDirtyLayout();
    } else if (value == Visibility::Invisible) {
        mFlags.set(WidgetFlag::WillNotDraw);
        markDirtyLayout();
    } else if (value == Visibility::Collapsed) {
        mFlags.set(WidgetFlag::WillNotDraw);
        markDirtyLayout();
    }
}

void Widget::setOpacity(float value) {
    float clamped = std::clamp(value, 0.0f, 1.0f);

    if (std::abs(mOpacity - clamped) < 0.0001f) { return; }

    mOpacity = clamped;

    invalidate();
}

void Widget::setEnable(bool enabled) {
    if (isEnabled() == enabled) { return; }
    mFlags.setFlag(ControlFlag::Disable, !enabled);
    invalidate();
}

void Widget::setHovered(bool hovered) {
    if (mIsHovered == hovered) { return; }
    mIsHovered = hovered;
    invalidate();
}

void Widget::setBorder(BorderPtr border) {
    mBorder = std::move(border);
}

void Widget::setBorderRadius(int radius) {
    if (!mBorder) {
        // mBorder = std::make_unique<Border>();
    }
    // mBorder->setRadius(radius);
}

void Widget::setBoundsClip(bool enable) {
    if (mEnableBoundsClip == enable) { return; }
    mEnableBoundsClip = enable;
}

void Widget::setClickable(bool clickable) {
    mFlags.setFlag(ControlFlag::Clickable, clickable);
}

void Widget::clearFocus() {
    // TODO not impl
}

void Widget::setBackground(const Color& color) {
    mBackground = std::make_unique<ColorDrawable>(color);
}

void Widget::setBackground(DrawablePtr drawable) {
    if (!drawable) {
        mBackground.reset();
        return;
    }
    mBackground = std::move(drawable);
}

void Widget::setBackgroundColor(const std::string& hexColorStr) {
    setBackground(Color::fromHexString(hexColorStr));
}

void Widget::discardCanvas() {
    if (mBackground) { mBackground->discardCanvas(); }
    if (mBorder) { mBorder->onDiscardCanvas(); }
}

void Widget::applyAttributes(const AttributeSet& attrs) {
    attrs.getString("id", mId);
    // attrs.getBool("enable", mEnable);
    // AttributeSet::getEnum<VisibleFlag>("visible", attrs, mVisible, parseToVisibleFlag);
}

void Widget::layout(const UIRect& pos) {
    if (mParent) {
        auto newPT = mParent->mPosTransform;
        mPosTransform = newPT.translate(static_cast<float>(pos.left()), static_cast<float>(pos.top()));
    }
    mPosition = pos;
    onLayout(pos);
}

void Widget::setMeasuredSize(const UISize& size) {
    if (!size.isValid()) { throw std::invalid_argument("invalid size"); }
    mMeasuredSize = size;
}

void Widget::drawBackground(Canvas& canvas) {
    // shadow bk state
    if (!mBackground) { return; }
    mBackground->setBounds(UIRect(0, 0, mMeasuredSize));
    mBackground->draw(&canvas);
}

void Widget::onDrawForeground(Canvas& canvas) {
    BIX_UNUSED(&canvas)

    if (mBorder) { mBorder->onDraw(mPosition, canvas); }
}

void Widget::onMeasure(Canvas& canvas, const UISize& available, const UISize& max) {
    BIX_UNUSED(canvas)
    BIX_UNUSED(max)
    if (!mSize.width.isAuto() && !mSize.height.isAuto()) {
        int w = mSize.width.get(available.width);
        int h = mSize.height.get(available.height);

        w = std::max(w, mMinSize.width);
        h = std::max(h, mMinSize.height);

        if (mMaxSize.width > 0) { w = std::min(w, mMaxSize.width); }
        if (mMaxSize.height > 0) { h = std::min(h, mMaxSize.height); }
        setMeasuredSize({w, h});
        return;
    }
    setMeasuredSize(available);
}

// bool Control::dispatchHoverEvent(const MouseEvent& event) {return false;}

void Widget::paint(Canvas& canvas) {
    if (mAlpha <= 0 || mMeasuredSize.isEmpty()) { return; }

    canvas.setTransform(mPosTransform);

    bool hasClip = false;
    if (mParent && mEnableBoundsClip) {
        if (mBorder) {
            hasClip = canvas.pushClip(mBorder->makeRect({0, 0, mMeasuredSize}));
        } else {
        }
    }

    drawBackground(canvas);

    onPaint(canvas);

    onDrawForeground(canvas);

    if (isContainer()) { dispatchPaint(canvas); }

    if (hasClip) { canvas.popClip(); }
}

void Widget::requestLayout() {

    BIX_ASSERT(!mFlags.testFlag(WidgetFlag::InLayout), "Recursive requestLayout() called during layout!");

    mFlags.on(WidgetFlag::DirtyLayout);

    // mFlags.set(ControlFlag::ForceLayout);

    if (mParent) { mParent->requestLayoutFromChild(this); }
}

void Widget::bindOnClick(const ClickCallback& callback) {
    if (!isClickable()) { setClickable(true); }
    mClickCallbacks.push_back(callback);
}

void Widget::handleMouseEvent(const MouseEvent& event) {

    if (event.ttype == WindowEventType::MouseLButtonDownEvent) {

        // fmt::println("down");
    }
}

// bool Control::mouseEnter() {
//     if (!mEnable||mVisible==VisibleFlag::Gone) {
//         return false;
//     }
// }

bool Widget::dispatchMouseMoveEvent(const MouseEvent& event) {

    auto hit = position().contains(event.position());
    if (!hit) {
        setHovered(false);
        return false;
    }
    setHovered(true);

    return onMouseHover(event);
}

bool Widget::dispatchHoverEvent(const MouseEvent& event) {

    if (!isEnabled()) {
        // TODO listerner
    }

    return onMouseHover(event);
}

bool Widget::onMouseHover(const MouseEvent& event) {
    BIX_UNUSED(event);
    return false;
}

void Widget::measure(Canvas& canvas, const UISize& available, const UISize& max) {

    if (max.width == 0 && max.height == 0) {
        setMeasuredSize({0, 0});
        return;
    }

    UISize limit = max;

    if (max.width < 0) { limit.width = mMaxSize.width; }
    if (max.height < 0) { limit.height = mMaxSize.height; }
    onMeasure(canvas, available, limit);
}

} // namespace bix

void bix::Widget::show() {
    setVisibility(Visibility::Visible);
}

void bix::Widget::hide() {
    setVisibility(Visibility::Collapsed);
}