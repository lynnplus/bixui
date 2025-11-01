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

#include "bixlib/control_names.h"
#include "bixlib/controls/drawable.h"
#include "bixlib/controls/length.h"
#include "bixlib/parser/attribute_set.h"

#include <string>

#define BIX_CTRL_NAME(name) bix::names::ClsName##name

#define BIX_DEFINE_CONTROL_CLASS_NAME(name)                                       \
    const std::string& className() const override { return BIX_CTRL_NAME(name); }

namespace bix {
// Flags used for controlling the paint
enum class ControlStateType {
    kControlStateNormal,
    kControlStateFocus,
    kControlStatePressed,
    kControlStateDisabled
};

enum class VisibleFlag {
    Gone,
    Visible,
    Invisible,
};

inline bool parseToVisibleFlag(const std::string& str, VisibleFlag& val) {
    if (str == "gone") {
        val = VisibleFlag::Gone;
    } else if (str == "visible") {
        val = VisibleFlag::Visible;
    } else if (str == "invisible") {
        val = VisibleFlag::Invisible;
    } else {
        return false;
    }
    return true;
}

struct BIX_PUBLIC Border {
    LineStyle lineStyle;
    Color color;
    int width;
    int radius;
};

class BIX_PUBLIC Control {
public:
    virtual ~Control() = default;

    virtual Control* parent() const;

    //******************getter****************//
    const std::string& id() const noexcept;
    // const std::string& name() const;
    VisibleFlag visible() const noexcept;
    const UIRect& margin() const noexcept;
    const UISize& measuredSize() const noexcept;
    const UIRect& padding() const noexcept;
    bool enabled() const noexcept;

    virtual bool isContainer() const;

    virtual const std::string& className() const { return BIX_CTRL_NAME(Control); }

    // set the current control to invalidate and trigger the redraw command
    void invalidate();

    void setParent(Control* parent);

    //******************set attrs********************//
    void setId(const std::string& id);
    void setMargin(const UIRect& margin);
    void setMargin(int v) { setMargin(UIRect(v, v, v, v)); }
    void setPadding(const UIRect& padding);
    void setPadding(int v) { setPadding(UIRect(v, v, v, v)); }
    void setSize(int w, int h) { setSize(UILength(w), UILength(h)); }
    void setSize(const UILength& w, const UILength& h);
    void setMaximumSize(const UISize& s);
    void setMinimumSize(const UISize& s);
    void setEnable(bool enabled);
    void setVisible(VisibleFlag flag);

    void setBorder(const Border& border);
    void setBorderWidth(int width);

    // Trim the drawing range of the control and do not draw beyond the range.
    void setBoundsClip(bool enable);
    /**
     *
     * @param alpha 0~255
     */
    void setAlpha(int alpha);

    virtual void setBackground(const Color& color);
    virtual void setBackground(DrawablePtr drawable);

    virtual void discardCanvas();
    /**
     * Apply styles or attributes to control
     * @param attrs
     */
    virtual void applyAttributes(const AttributeSet& attrs);

protected:
    friend class ControlHelper;

    bool mEnable = true;
    VisibleFlag mVisible = VisibleFlag::Visible;
    UIRect mPadding{0, 0, 0, 0};
    UIRect mMargin{0, 0, 0, 0};
    int mAlpha = 255;
    Control* mParent = nullptr;
    UIRect mPosition{};
    UISize mMaxSize{};
    UISize mMinSize{};
    SpecSize mSize{};
    UISize mMeasuredSize{};
    Border mBorder{LineStyle::Solid, colors::Black, 0, 0};

    Transform mPosTransform{};
    std::string mId{};

    // virtual void onRenderCreated(Canvas* renderer){BIX_UNUSED(renderer)}

    /**
     *
     * @param rect relative to parent
     */
    virtual void layout(const UIRect& rect);
    virtual void onDraw(Canvas& canvas) { BIX_UNUSED(canvas) }
    void drawBackground(Canvas& canvas);
    virtual void onDrawForeground(Canvas& canvas);
    virtual void dispatchDraw(Canvas& canvas) { BIX_UNUSED(canvas) }
    virtual void onLayout(const UIRect& rect) { BIX_UNUSED(rect) }
    virtual void onMeasure() {}
    virtual void onRemoved() {}
    // Discard DeviceResources
private:
    DrawablePtr mBackground = nullptr;
    PenPtr mBorderPen = nullptr;
    void draw(Canvas& renderer);
};

using ControlPtr = std::unique_ptr<Control>;
} // namespace bix