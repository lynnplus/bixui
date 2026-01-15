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

#include "bixlib/widgets/container.h"

#include "window/window_private.h"

#include <bixlib/utils/fmt_bix.h>
#include <bixlib/widgets/button.h>

#include <stdio.h>
#include <windows.h>

#include <memory>

// namespace bix::ui {
//
// // void Layout::onLayout(const UIRect& pos) {
// //     const auto content = pos - getPaddingWithForeground();
// //     UIRect temp{};
// //     for (const auto& item : mChildren) {
// //         const auto& margins = item->margins();
// //         temp.x1 = content.left() + margins.left;
// //         temp.y1 = content.top() + margins.top;
// //         temp.setSize(item->measuredSize());
// //         item->layout(temp);
// //     }
// // }
// //
// // void Layout::onMeasure(Canvas& canvas, const UISize& available, const UISize& max) {
// //     auto pad = getPaddingWithForeground();
// //     const auto contentSize = mSize.get(available) - pad;
// //     std::vector<Widget*> reMeasures;
// //
// //     UISize temp{}, maxSize{0, 0};
// //     for (const auto& item : mChildren) {
// //         const auto& margins = item->margins();
// //         temp = contentSize - margins;
// //         item->measure(canvas, temp, max);
// //         auto m = item->measuredSize();
// //
// //         maxSize.width = std::max(maxSize.width + margins.totalX(), m.width);
// //         maxSize.height = std::max(maxSize.height + margins.totalY(), m.height);
// //
// //         auto size = item->size();
// //         if (size.width.isRelative() || size.height.isRelative()) {
// //             reMeasures.push_back(item.get());
// //         }
// //     }
// //     if (contentSize.width > 0) {
// //         maxSize.width = contentSize.width;
// //     }
// //     if (contentSize.height > 0) {
// //         maxSize.height = contentSize.height;
// //     }
// //
// //     for (const auto item : reMeasures) {
// //         const auto& margins = item->margins();
// //         temp = maxSize - margins;
// //         item->measure(canvas, temp, max);
// //     }
// //
// //     // fmt::println("layout {}",(maxSize+pad));
// //     setMeasuredSize(maxSize + pad);
// // }
// //
// // void Layout::dispatchDraw(Canvas& canvas) {
// //     if (mChildren.empty()) {
// //         return;
// //     }
// //     for (const auto& item : mChildren) {
// //         item->draw(canvas);
// //     }
// // }
// //
// // void Layout::dispatchRemoved(Widget* child) { ControlHelper::removed(*child); }
// //
// // bool Layout::dispatchMouseMoveEvent(const MouseEvent& event) {
// //     auto& pos = event.position();
// //     if (Widget::dispatchMouseMoveEvent(event)) {
// //         return true;
// //     }
// //     return utils::anyOf(mChildren, [&pos, &event](ControlPtr& item) {
// //         auto hit = item->position().contains(pos);
// //         if (!hit && !item->isHovered()) {
// //             return false;
// //         }
// //         return item->dispatchMouseMoveEvent(event);
// //     });
// // }
//
// WidgetPtr Container::removeChild(Widget* child) {
//     if (!child)
//         return nullptr;
//
//     auto it = std::ranges::find_if(mChildren, [child](const WidgetPtr& p) { return p.get() == child; });
//
//     return (it != mChildren.end()) ? removeChildImpl(it) : nullptr;
// }
//
// WidgetPtr Container::removeChildAt(int index) {
//     return isValidIndex(index) ? removeChildImpl(mChildren.begin() + index) : nullptr;
// }
//
// void Container::clearChildren() {
//     if (mChildren.empty()) {
//         return;
//     }
//     // TODO 批量解绑父子关系
//     for (auto& child : mChildren) {
//         if (child) {
//             child->setParent(nullptr);
//         }
//     }
//
//     mChildren.clear(); // 触发所有 unique_ptr 批量析构
//     invalidate();
// }
//
// WidgetPtr Container::removeChildImpl(ChildIterator it) {
//     // 1. 夺取所有权
//     WidgetPtr removed = std::move(*it);
//
//     // 2. 环境清理
//     if (removed) {
//         removed->setParent(nullptr);
//     }
//
//     // 3. 物理移除
//     mChildren.erase(it);
//     invalidate();
//     // TODO dispatchRemoved(tmp.get());
//     return removed;
// }
//
// Widget* Container::findByIdImpl(std::string_view id) const {
//     for (const auto& child : mChildren) {
//         if (!child) {
//             continue;
//         }
//         if (child->id() == id) {
//             return child.get();
//         }
//         if (child->isContainer()) {
//             const auto* container = static_cast<const Container*>(child.get());
//             if (auto* found = container->findByIdImpl(id)) {
//                 return found;
//             }
//         }
//     }
//     return nullptr;
// }
//
// int Container::childIndex(const Widget* child) const {
//     auto it = std::ranges::find_if(mChildren, [child](const WidgetPtr& p) { return p.get() == child; });
//
//     if (it != mChildren.end()) {
//         return static_cast<int>(std::distance(mChildren.begin(), it));
//     }
//     return -1;
// }
//
// Widget* Container::childAt(int index) const { return isValidIndex(index) ? mChildren[index].get() : nullptr; }
//
// Widget* Container::addChildImpl(WidgetPtr child, int index) {
//     if (!child) {
//         return nullptr;
//     }
//
//     Widget* rawPtr = child.get();
//     child->setParent(this);
//
//     if (isValidIndex(index)) {
//         mChildren.insert(mChildren.begin() + index, std::move(child));
//     } else {
//         mChildren.push_back(std::move(child));
//     }
//
//     this->invalidate();
//     return rawPtr;
// }
// } // namespace bix::ui
