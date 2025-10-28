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
#include <vector>
#include "control.h"


namespace bix {
class BIX_PUBLIC Layout : public Control {
    //padding border margin  总宽度=width + padding*2 + border*2 + margin*2
public:
    BIX_DEFINE_CONTROL_CLASS_NAME(Layout)

    Control* add(ControlPtr control);
    ControlPtr removeAt(int index);
    void removeAll();
    size_t childrenCount() const;
    bool isContainer() const final;
    Control* findByName(const std::string& name) const;

    void requestLayout();

    void discardCanvas() override;

protected:
    std::vector<ControlPtr> mChildren;

    void onLayout(const UIRect& rect) override =0;
    void dispatchDraw(Canvas& renderer) override;
    void dispatchRemoved(Control* removed);

private:
    // Renderer* mTmpRenderer=nullptr;
    //padding  background(color bitmap svg)
};

using LayoutPtr = std::unique_ptr<Layout>;
} // bix
