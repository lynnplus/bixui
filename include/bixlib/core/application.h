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

#include "bixlib/export_macro.h"
#include "bixlib/render/engine.h"

#define main bixUserMain

int bixUserMain(int argc, char* argv[]);

namespace bix {
class AppPrivate;

class BIX_PUBLIC ApplicationCtx {
public:
    virtual ~ApplicationCtx() = default;
    virtual RenderEngine* getUIRenderer() =0;
};


class BIX_PUBLIC Application {
public:
    Application();
    Application(int argc, char* argv[]);
    virtual ~Application() = default;


    void setRenderEngine(RenderEngine::Type type);

    int run();

protected:
    virtual void onThemeChanged(int flag) { BIX_UNUSED(flag) }

    virtual void onCreate() {}
    virtual void onDestroy() {}

private:
    friend class AppPrivate;
    AppPrivate* mPrivate = nullptr;
};

ApplicationCtx* getAppContext();
}