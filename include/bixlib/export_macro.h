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

#define BIX_UNUSED(x) (void)x;

#if defined _WIN32 || defined __CYGWIN__
    #define BIX_DECLARE_EXPORT __declspec(dllexport)
    #define BIX_DECLARE_IMPORT __declspec(dllimport)
    #define BIX_DECLARE_PRIVATE
    #define BIX_DECLARE_DEPRECATED __declspec(deprecated)
#else
    #define BIX_DECLARE_EXPORT __attribute__((visibility("default")))
    #define BIX_DECLARE_IMPORT _BIX_DECLARE_EXPORT
    #define BIX_DECLARE_PRIVATE __attribute__((visibility("hidden")))
    #define BIX_DECLARE_DEPRECATED __attribute__((deprecated))
#endif

#ifdef BIX_STATIC_DEFINE
    #define BIX_PUBLIC
    #define BIX_PRIVATE
#else
    #define BIX_PRIVATE BIX_DECLARE_PRIVATE
    #ifdef BIX_EXPORTS
        #define BIX_PUBLIC BIX_DECLARE_EXPORT
    #else
        #define BIX_PUBLIC BIX_DECLARE_IMPORT
    #endif
#endif