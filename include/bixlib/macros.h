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

/**
 * @defgroup bix_macros Bixlib Macros
 * @brief A complete collection of preprocessor utilities for framework-wide use.
 * @details This module serves as a centralized hub for all public macros provided by
 * the Bixlib framework. These macro tools are designed to:
 * - **Simplify Boilerplate**: Reduce repetitive code during widget and type registration.
 * - **Enhance Type Safety**: Bridge the gap between low-level C++ preprocessor and
 * high-level template logic (e.g., @ref BIX_DECLARE_ENUM_FLAGS).
 * - **Standardize Development**: Provide a unified set of tools that ensure
 * consistency across different modules of the application.
 * Developers should use these macros to integrate their custom types into the
 * framework's ecosystem efficiently.
 */

#define BIX_DISABLE_COPY_AND_MOVE(ClassName)         \
    ClassName(const ClassName&) = delete;            \
    ClassName& operator=(const ClassName&) = delete; \
    ClassName(ClassName&&) = delete;                 \
    ClassName& operator=(ClassName&&) = delete;

#define BIX_INTERFACE(ClassName)         \
    virtual ~ClassName();                \
    BIX_DISABLE_COPY_AND_MOVE(ClassName)