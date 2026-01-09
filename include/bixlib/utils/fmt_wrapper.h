/*
 * Copyright (c) 2026 Lynn <lynnplus90@gmail.com>
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

#undef BIX_HAS_LIBRARY

#if defined(BIX_FMT_HEADER)
    #define BIX_HAS_LIBRARY
    #include BIX_FMT_HEADER
#elif __has_include(<fmt/format.h>)
    #define BIX_HAS_LIBRARY
    #include <fmt/format.h>
#elif __has_include(<spdlog/fmt/fmt.h>)
    #define BIX_HAS_LIBRARY
    #include <spdlog/fmt/fmt.h>
#endif
