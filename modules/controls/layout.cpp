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

#include "bixlib/controls/layout.h"

#include "bixlib/control_names.h"
#include "bixlib/utils/fmt_bix.h"
#include "bixlib/utils/foreach.h"
#include "control_helper.h"

namespace bix {
const std::string& Layout::className() const noexcept { return names::ClsNameLayout; }

Control* Layout::add(ControlPtr control, int index) {
    if (!control)
        return nullptr;

    // TODO index impl
    BIX_UNUSED(index)
    control->setParent(this);
    control->setWindow(mWindow);

    mChildren.push_back(std::move(control));
    return mChildren.back().get();
}

ControlPtr Layout::removeAt(int index) {
    if (static_cast<int>(mChildren.size()) <= index) {
        return nullptr;
    }
    auto it = mChildren.begin() + index;
    auto tmp = std::move(*it);
    mChildren.erase(it);

    tmp->setParent(nullptr);
    tmp->setWindow(nullptr);
    tmp->discardCanvas();
    dispatchRemoved(tmp.get());
    return tmp;
}

void Layout::removeAll() {
    for (const auto& item : mChildren) {
        item->setParent(nullptr);
        item->setWindow(nullptr);
        item->discardCanvas();
    }
    mChildren.clear();
}

size_t Layout::childrenCount() const { return mChildren.size(); }

Control* Layout::findByName(const std::string& name) const {
    if (mChildren.empty() || name.empty()) {
        return nullptr;
    }
    for (const auto& item : mChildren) {
        if (item->id() == name) {
            return item.get();
        }
        // TODO If the control to be searched is on this layer, is it inefficient?
        if (item->isContainer()) {
            const auto v = static_cast<Layout*>(item.get())->findByName(name);
            if (v != nullptr) {
                return v;
            }
        }
    }
    return nullptr;
}

void Layout::setWindow(Window* window) {
    Control::setWindow(window);
    for (const auto& item : mChildren) {
        item->setWindow(window);
    }
}

void Layout::onLayout(const UIRect& pos) {
    const auto content = pos - getPaddingWithForeground();
    UIRect temp{};
    for (const auto& item : mChildren) {
        const auto& margins = item->margins();
        temp.x1 = content.left() + margins.left;
        temp.y1 = content.top() + margins.top;
        temp.setSize(item->measuredSize());
        item->layout(temp);
    }
}

void Layout::onMeasure(Canvas& canvas, const UISize& available, const UISize& max) {
    auto pad = getPaddingWithForeground();
    const auto contentSize = mSize.get(available) - pad;
    std::vector<Control*> reMeasures;

    UISize temp{}, maxSize{0, 0};
    for (const auto& item : mChildren) {
        const auto& margins = item->margins();
        temp = contentSize - margins;
        item->measure(canvas, temp, max);
        auto m = item->measuredSize();

        maxSize.width = std::max(maxSize.width + margins.totalX(), m.width);
        maxSize.height = std::max(maxSize.height + margins.totalY(), m.height);

        auto size = item->size();
        if (size.width.isRelative() || size.height.isRelative()) {
            reMeasures.push_back(item.get());
        }
    }
    if (contentSize.width > 0) {
        maxSize.width = contentSize.width;
    }
    if (contentSize.height > 0) {
        maxSize.height = contentSize.height;
    }

    for (const auto item : reMeasures) {
        const auto& margins = item->margins();
        temp = maxSize - margins;
        item->measure(canvas, temp, max);
    }

    // fmt::println("layout {}",(maxSize+pad));
    setMeasuredSize(maxSize + pad);
}

void Layout::dispatchDraw(Canvas& canvas) {
    if (mChildren.empty()) {
        return;
    }
    for (const auto& item : mChildren) {
        item->draw(canvas);
    }
}

void Layout::dispatchRemoved(Control* child) { ControlHelper::removed(*child); }

bool Layout::dispatchMouseMoveEvent(const MouseEvent& event) {
    auto& pos = event.position();
    if (Control::dispatchMouseMoveEvent(event)) {
        return true;
    }
    return utils::anyOf(mChildren, [&pos, &event](ControlPtr& item) {
        auto hit = item->position().contains(pos);
        if (!hit && !item->isHovered()) {
            return false;
        }
        return item->dispatchMouseMoveEvent(event);
    });
}

void Layout::requestLayout() {}

void Layout::discardCanvas() {
    Control::discardCanvas();
    for (const auto& item : mChildren) {
        item->discardCanvas();
    }
}

// bool Layout::dispatchMouseEvent(const MouseEvent& event) {
//     if (mVisible==VisibleFlag::Gone) {
//         return false;
//     }
//
//
//
//
//
//     std::for_each(mChildren.rbegin(),mChildren.rend(),[](auto& item) {
//
//
//
//
//     });
//
//
//
//
//     return false;
// }

bool Layout::isContainer() const noexcept { return true; }
} // namespace bix
