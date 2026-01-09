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

// #define WIN32_LEAN_AND_MEAN
#include <windows.h>

namespace bix {

/**
 * Windows-specific window information.
 * This structure implements the forward-declared NativeWindowInfo from Window.h.
 */
struct NativeWindowInfo {
    /**
     * The main window handle.
     */
    HWND hwnd = nullptr;

    /**
     * The device context for the window's client area.
     * @note Useful for GDI or legacy OpenGL initialization.
     */
    HDC hdc = nullptr;

    /**
     * Optional: The class name used to register this window.
     */
    const char* className = nullptr;
};

} // namespace bix