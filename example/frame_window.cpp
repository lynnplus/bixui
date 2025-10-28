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

#include "frame_window.h"

#include "bixlib/controls/button.h"
#include "bixlib/controls/label.h"
#include "bixlib/controls/linear_layout.h"

FrameWindow::FrameWindow() {
    setUniqueId("FrameWindow");

    auto ll = std::make_unique<bix::LinearLayout>();

    auto btn1 = std::make_unique<bix::Label>();
    auto btn2 = std::make_unique<bix::Button>();
    auto btn3 = std::make_unique<bix::Button>();

    btn1->setSize(200, 400);
    btn2->setSize(100, 200);
    btn3->setSize(100, 200);

    // btn1.setText();

    btn1->setText("hello");
    btn1->setBorderWidth(1);
    // btn1->setMargin(10);

    btn1->setPadding(10);

    ll->add(std::move(btn1));
    // ll->add(std::move(btn2));
    // ll->add(std::move(btn3));

    setLayout(std::move(ll));

    setClientSize(801, 600);

    // resize(bix::AutoParent,bix::AutoParent);
}

// std::string FrameWindow::uniqueId() const {
//     return "123";
// }
//
// bool FrameWindow::onClose() {
//
//     fmt::println("window close ok");
//
//     return Window::onClose();
// }
//
// void FrameWindow::onDestroy() {
//     Window::onDestroy();
//
//     fmt::println("window destroy ok");
// }
