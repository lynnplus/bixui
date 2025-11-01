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

#include "bixlib/controls/layout.h"
#include "bixlib/controls/length.h"
#include "bixlib/core/display.h"
#include "bixlib/export_macro.h"

#include <memory>

namespace bix {
class WindowPrivate;

class BIX_PUBLIC Window {
public:
    enum State {
        WindowNormal = 0,
        WindowMinimized = 1,
        WindowMaximized = 2,
        WindowFullScreen = 4,
        WindowActive = 8
    };

    Window();
    virtual ~Window();

    intptr_t nativeHandle() const;

    void show();
    void resize(const UILength& w, const UILength& h);
    void resize(int w, int h);
    /**
     * Set window content area size
     * @param w
     * @param h
     */
    void setClientSize(const UILength& w, const UILength& h);
    void setClientSize(int w, int h);

    void moveToCenter();
    void setTitle(const std::string& title);

    WindowPrivate* getImpl() const;

    std::string title() const;

    std::string uniqueId() const;
    void setUniqueId(const std::string& id);
    void setDisplayStrategy(DisplayStrategyCRef strategy);

    DisplayStrategyPtr displayStrategy() const;

    DisplayPtr display() const;

protected:
    // listening
    virtual void onBuildFinish() {}
    virtual void onCreate() {}
    virtual void onAttach() {}
    virtual void onShow() {}
    virtual void onResize() {}

    virtual bool onClose() { return false; }
    virtual void onDestroy() {}

    void setLayout(std::unique_ptr<Layout> root);

private:
    friend class WindowPrivate;
    std::unique_ptr<WindowPrivate> mPrivate;
};
} // namespace bix