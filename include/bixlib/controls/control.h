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

#include "bixlib/controls/drawable.h"
#include "bixlib/controls/length.h"
#include "bixlib/core/window_events.h"
#include "bixlib/parser/attribute_set.h"
#include "bixlib/utils/flags.h"
#include "bixlib/controls/border.h"

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

enum class VisibleFlag {
    Gone,
    Visible,
    Invisible,
};

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

enum class ControlFlag : uint32_t {
    Disable =       1 << 0,
    Clickable =     1 << 1,
    Invalidated=    1 << 2,
    ForceLayout=    1<<3
};
BIX_DECLARE_ENUM_FLAGS(ControlFlag)
using ControlFlags = Flags<ControlFlag>;

// TODO move to other
class Control;

using ClickCallback = std::function<void(Control& target)>;

class BIX_PUBLIC Control {
public:
    virtual ~Control() = default;

    Control* parent() const noexcept;

    //******************getter****************//
    const std::string& id() const noexcept;
    VisibleFlag visible() const noexcept;
    const UIMargins& margins() const noexcept;
    const SpecSize& size() const noexcept;
    const UISize& measuredSize() const noexcept;
    const UIPaddings& padding() const noexcept;
    bool isEnabled() const noexcept;
    bool isHovered() const noexcept;
    bool isClickable() const noexcept;
    const UIRect& position() const noexcept;

    Border* border() const noexcept;

    virtual bool isContainer() const noexcept;

    virtual const std::string& className() const noexcept = 0;
    virtual UIPaddings getPaddingWithForeground() const noexcept;

    // set the current control to invalidate and trigger the redraw command
    void invalidate();

    void setParent(Control* parent);
    virtual void setWindow(Window* window);

    //******************set attrs********************//
    void setId(const std::string& id);
    void setMargins(const UIMargins& margin);

    void setMargins(int v) { setMargins(UIMargins(v)); }

    void setPadding(const UIPaddings& padding);

    void setPadding(int v) { setPadding(UIPaddings(v)); }

    void setSize(int w, int h) { setSize(UILength(w), UILength(h)); }

    void setSize(const UILength& w, const UILength& h);

    void setMaximumSize(const UISize& s);
    void setMinimumSize(const UISize& s);
    void setEnable(bool enabled);
    void setVisible(VisibleFlag flag);
    void setHovered(bool hovered);

    void setBorder(BorderPtr border);

    // Trim the drawing range of the control and do not draw beyond the range.
    void setBoundsClip(bool enable);
    /**
     *
     * @param alpha 0~255
     */
    void setAlpha(int alpha);
    void setClickable(bool clickable);

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

    void layout(const UIRect& pos);

    /**
     * measure the actual size of the control
     * @param canvas
     * @param available available size of parent container
     * @param max the maximum size allowed by the parent container. A value less than 0 means there is no limit.
     */
    void measure(Canvas& canvas, const UISize& available, const UISize& max);

    void draw(Canvas& canvas);

    void bindOnClick(const ClickCallback& callback);

    void handleMouseEvent(const MouseEvent& event);

    virtual bool dispatchMouseMoveEvent(const MouseEvent& event);

protected:
    friend class ControlHelper;

    bool mEnableBoundsClip = true;
    bool mIsHovered = false;
    VisibleFlag mVisible = VisibleFlag::Visible;
    UIPaddings mPadding{0};
    UIMargins mMargin{0};
    int mAlpha = 255;
    Control* mParent = nullptr;
    Window* mWindow = nullptr;
    UISize mMaxSize{-1, -1};
    UISize mMinSize{0, 0};
    SpecSize mSize{};

    void setMeasuredSize(const UISize& size);

    // virtual UIRect calculateContentRect(const UIRect& pos) const noexcept;

    virtual void onDraw(Canvas& canvas) { BIX_UNUSED(canvas) }

    virtual void drawBackground(Canvas& canvas);
    virtual void onDrawForeground(Canvas& canvas);

    virtual void dispatchDraw(Canvas& canvas) { BIX_UNUSED(canvas) }

    virtual void onLayout(const UIRect& rect) { BIX_UNUSED(rect) }

    virtual void onMeasure(Canvas& canvas, const UISize& available, const UISize& max);

    virtual void onRemoved() {}

    virtual bool dispatchHoverEvent(const MouseEvent& event);

    virtual bool onMouseHover(const MouseEvent& event);

    // virtual  void onMouseEnter();
    // virtual  void onMouseLeave();

    // Discard DeviceResources
private:
    std::string mId{};
    UIRect mPosition{}; // layout position
    UISize mMeasuredSize{0, 0};
    BorderPtr mBorder=nullptr;
    Transform mPosTransform{};
    DrawablePtr mBackground = nullptr;
    ControlFlags mFlags;
    std::vector<ClickCallback> mClickCallbacks;
};

using ControlPtr = std::unique_ptr<Control>;
} // namespace bix