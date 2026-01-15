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

#include <bixlib/controls/border.h>
#include <bixlib/controls/drawable.h>
#include <bixlib/core/insets.h>
#include <bixlib/core/window_events.h>
#include <bixlib/parser/attribute_set.h>
#include <bixlib/utils/flags.h>
#include <bixlib/widgets/view_parent.h>
#include <bixlib/widgets/widget_defs.h>

#include <string>

namespace bix {

class Window;

/**
 * @file control.h
 * @enum ControlState
 * Represents the various visual and interaction states of a UI basic control.
 *
 * This enum class defines the possible states a control can be in, using bit flags
 * to allow combinations of states (e.g., a control can be both Focused and Hovered).
 * The values are defined as bit shifts to facilitate bitwise operations.
 */
enum class ControlState : uint32_t {
    Normal = 0,        ///< The default, inactive state of the control.
    Hovered = 1 << 0,  ///< The control is currently being hovered over by the mouse cursor.
    Focused = 1 << 1,  ///< The control currently has keyboard focus.
    Pressed = 1 << 2,  ///< The control is being pressed (e.g., mouse button down).
    Disabled = 1 << 3, ///< The control is disabled and non-interactive.
};
BIX_DECLARE_ENUM_FLAGS(ControlState)
using ControlStates = Flags<ControlState>;

// AlignType
enum class gravity {
    Left, // left on ltr
    Right,
    Top,
    Bottom,
    Center,
    center_horizontal,
    center_vertical
};

class Widget;

/**
 * A type alias for a unique pointer to a Widget.
 *
 * This represents unique ownership of a Widget instance. Use this when
 * passing widgets into containers or transferring ownership between objects.
 */
using WidgetPtr = std::unique_ptr<Widget>;

using ClickCallback = std::function<void(Widget& target)>;

class BIX_PUBLIC Widget {
public:
    virtual ~Widget() = default;

    static constexpr const char* StaticType() { return "Widget"; }

    virtual const char* typeName() const noexcept { return StaticType(); }

    template <typename T>
    bool is() const noexcept {
        return this->typeName() == T::StaticType();
    }

    template <typename T>
    T* as() noexcept {
        return is<T>() ? static_cast<T*>(this) : nullptr;
    }

    template <typename T>
    const T* as() const noexcept {
        return is<T>() ? static_cast<const T*>(this) : nullptr;
    }

    void show();

    void hide();
    void layout(const UIRect& pos);
    void paint(Canvas& canvas);
    void requestLayout();

    Length width() const noexcept { return mWidth; }

    Length height() const noexcept { return mHeight; }

    const std::string& id() const noexcept { return mId; }

    ViewParent* parent() const noexcept { return mParent; }

    const EdgeInsets& margins() const noexcept { return mMargin; }

    const EdgeInsets& padding() const noexcept { return mPadding; }

    Size measuredSize() const noexcept { return mMeasuredSize; }

    /**
     * Gets the current opacity of the widget.
     * @return A float value between 0.0 (transparent) and 1.0 (opaque).
     */
    float opacity() const noexcept { return mOpacity; }

    /**
     * Gets the current visibility of the widget.
     * @return The Visibility state (Visible, Invisible, or Collapsed).
     */
    Visibility visibility() const noexcept { return mVisibility; }

    bool isEnabled() const noexcept;
    bool isHovered() const noexcept;
    bool isClickable() const noexcept;
    const UIRect& position() const noexcept;

    Border* border() const noexcept;

    void invalidate();

    void setParent(ViewParent* parent);

    //******************set attrs********************//

    void setWidth(Length width);
    void setHeight(Length height);
    void setMargins(const EdgeInsets& margin);
    void setPadding(const EdgeInsets& padding);
    void setMaximumSize(Length w, Length h);
    void setMinimumSize(Length w, Length h);

    /**
     * Sets the visibility of the widget.
     * @param value The new visibility state.
     * @note If the state changes to or from 'Collapsed', a relayout of the parent is triggered.
     */
    void setVisibility(Visibility value);
    /**
     * Sets the opacity of the widget.
     * @param value The target opacity (0.0 to 1.0).
     */
    void setOpacity(float value);
    // TODO
    //  Length maxWidth() const { return mConstraints.maxWidth; }
    //  Length minWidth() const { return mConstraints.minWidth; }

    void setEnable(bool enabled);
    // void setVisible(VisibleFlag flag);
    void setHovered(bool hovered);

    void setBorder(BorderPtr border);
    void setBorderRadius(int radius);

    // Trim the drawing range of the control and do not draw beyond the range.
    void setBoundsClip(bool enable);

    void setClickable(bool clickable);

    virtual bool isContainer() const noexcept { return false; }

    // virtual UIPaddings getPaddingWithForeground() const noexcept;
    virtual void clearFocus();

    virtual void setBackground(const Color& color);
    virtual void setBackground(DrawablePtr drawable);
    virtual void setBackgroundColor(const std::string& hexColorStr);

    virtual void discardCanvas();
    /**
     * Apply styles or attributes to control
     * @param attrs
     */
    virtual void applyAttributes(const AttributeSet& attrs);

    /**
     * measure the actual size of the control
     * @param canvas
     * @param available available size of parent container
     * @param max the maximum size allowed by the parent container. A value less than 0 means there is no limit.
     */
    // void measure(Canvas& canvas, const UISize& available, const UISize& max);

    void bindOnClick(const ClickCallback& callback);

    void handleMouseEvent(const MouseEvent& event);

    virtual bool dispatchMouseMoveEvent(const MouseEvent& event);

protected:
    bool mEnableBoundsClip = true;
    bool mIsHovered = false;

    // VisibleFlag mVisible = VisibleFlag::Visible;

    // void setMeasuredSize(const UISize& size);

    // virtual UIRect calculateContentRect(const UIRect& pos) const noexcept;

    virtual void dispatchPaint(Canvas& canvas) { BIX_UNUSED(canvas) }

    virtual void onPaint(Canvas& canvas) = 0;
    virtual void paintBackground(Canvas& canvas);
    virtual void paintForeground(Canvas& canvas);

    virtual void onLayout(const UIRect& rect) { BIX_UNUSED(rect) }

    // virtual void onMeasure(Canvas& canvas, const UISize& available, const UISize& max);

    virtual void onRemoved() {}

    virtual bool dispatchHoverEvent(const MouseEvent& event);

    virtual bool onMouseHover(const MouseEvent& event);

    // virtual  void onMouseEnter();
    // virtual  void onMouseLeave();

    // Discard DeviceResources
private:
    std::string mId{};
    Length mWidth{Length::autoSize()};
    Length mHeight{Length::autoSize()};

    BoxConstraints mConstraints{};

    EdgeInsets mPadding{};
    EdgeInsets mMargin{};

    Rect mBounds;

    UIRect mPosition{}; // layout position
    Size mMeasuredSize{-1, -1};
    BorderPtr mBorder = nullptr;
    Transform mPosTransform{};
    DrawablePtr mBackground = nullptr;
    float mOpacity = 1.0;
    Visibility mVisibility = Visibility::Visible;
    WidgetFlags mFlags{};
    // ControlFlags mFlags;
    ViewParent* mParent = nullptr;
    std::vector<ClickCallback> mClickCallbacks;
};

class LeafWidget : public Widget {
public:
    bool isContainer() const noexcept final { return false; }

protected:
    void dispatchPaint(Canvas& canvas) final { BIX_UNUSED(canvas) }
};

// --- Second layer: CRTP middleware (processing chain calls) ---
template <typename T>
class WidgetTemplate : public Widget {
public:
};

} // namespace bix