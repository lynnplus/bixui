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
#include <bixlib/assert.h>
#include <bixlib/widgets/widget.h>
#include <bixlib/widgets/widget_macros.h>

#include <vector>

namespace bix {

class BIX_PUBLIC Container : public Widget, public ViewParent {

public:
    BIX_WIDGET_DECLARE(Container)

    using ChildList = std::vector<WidgetPtr>;
    using ChildIterator = ChildList::iterator;

    bool isContainer() const noexcept override { return true; }

    Widget* addChild(WidgetPtr child, int index = -1) { return addChildImpl(std::move(child), index); }

    template <DerivedFrom<Widget> T>
    T* addChild(std::unique_ptr<T> child, int index = -1) {
        T* ptr = child.get();
        addChildImpl(std::move(child), index);
        return ptr;
    }

    template <DerivedFrom<Widget> T, typename... Args>
    requires(!traits::UniquePtrType<traits::FirstArgT<Args...>>)
    T* addChild(Args&&... args) {
        return addChild(std::make_unique<T>(std::forward<Args>(args)...));
    }

    template <DerivedFrom<Widget> T, typename... Args>
    T* insertChild(int index, Args&&... args) {
        return addChild(std::make_unique<T>(std::forward<Args>(args)...), index);
    }

    template <typename T = Widget>
    [[nodiscard]] T* findById(std::string_view id) const {
        Widget* raw = findByIdImpl(id);
        if (!raw) { return nullptr; }
        T* target = dynamic_cast<T*>(raw);
        BIX_ASSERT(target != nullptr, "Widget ID '{}' was found, but its type is not '{}'", id, typeid(T).name());
        return target;
    }

    WidgetPtr removeChild(Widget* child);
    WidgetPtr removeChildAt(int index);
    void clearChildren();

    size_t childCount() const noexcept { return mChildren.size(); }

    bool isEmpty() const noexcept { return mChildren.empty(); }

    int childIndex(const Widget* child) const;

    Widget* childAt(int index) const;

    // bool dispatchMouseEvent(const MouseEvent& event) override;
    // bool dispatchMouseMoveEvent(const MouseEvent& event) override;

protected:
    ChildList mChildren;

    Widget* addChildImpl(WidgetPtr child, int index);
    WidgetPtr removeChildImpl(std::vector<WidgetPtr>::iterator it);
    Widget* findByIdImpl(std::string_view id) const;

    bool isValidIndex(int index) const noexcept { return index >= 0 && index < static_cast<int>(mChildren.size()); }

    // void onLayout(const UIRect& pos) override;
    // void onMeasure(Canvas& canvas, const UISize& available, const UISize& max) override;
    // void dispatchDraw(Canvas& renderer) override;
    // void dispatchRemoved(Widget* removed);
};

} // namespace bix
