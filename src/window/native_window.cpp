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

#include "window/native_window.h"

#include <bixlib/export_macro.h>

namespace bix {

namespace {

class NativeWindowDummy : public NativeWindow {
public:
    void createNative() override {}

    void destroyNative() override {}

    ScreenPtr getScreen() const override { return nullptr; }

    bool queryNativeInfo(NativeWindowInfo& info) const override { BIX_UNUSED(info) return false; }

    void setTitle(std::string_view) override {}
};
} // namespace

std::unique_ptr<NativeWindow> NativeWindow::createDummy() {
    return std::make_unique<NativeWindowDummy>();
}
} // namespace bix
