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
#include "control.h"

#include <vector>

namespace bix {

/**
 * Layout is a special control that can contain other controls (children-controls)
 * and is also the base class of the layout container.
 * Layout draws child controls overlapping by default.
 */
class BIX_PUBLIC Layout : public Control {
public:

    using ChildrenList = std::vector<ControlPtr>;

    const std::string& className() const noexcept override;

    Control* add(ControlPtr control,int index=-1);

    template <class T>
    T* add2(std::unique_ptr<T> control,int index=-1) {
        T* p=control.get();
        add(std::move(control),index);
        return p;
    }

    template <DerivedFrom<Control> T, class... Args>
    T* add3(Args&&... args) {
        return add2(std::make_unique<T>(std::forward<Args>(args)...));
    }

    ControlPtr removeAt(int index);
    void removeAll();
    size_t childrenCount() const;
    bool isContainer() const noexcept final;
    Control* findByName(const std::string& name) const;
    void setWindow(Window* window) override;
    void requestLayout();

    void discardCanvas() override;

    // bool dispatchMouseEvent(const MouseEvent& event) override;
    bool dispatchMouseMoveEvent(const MouseEvent& event) override;
protected:


    ChildrenList mChildren;

    void onLayout(const UIRect& pos) override;
    void onMeasure(Canvas& canvas, const UISize& available, const UISize& max) override;
    void dispatchDraw(Canvas& renderer) override;
    void dispatchRemoved(Control* removed);



private:
    // Renderer* mTmpRenderer=nullptr;
    // padding  background(color bitmap svg)
};

using LayoutPtr = std::unique_ptr<Layout>;
} // namespace bix
