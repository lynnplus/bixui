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
#include <exception>
#include <stdexcept>

namespace bix {
class Error : public std::runtime_error {
public:

    explicit Error(const std::string& message): std::runtime_error(message) {}

    explicit Error(const char* message): std::runtime_error(message) {}


    template<typename... Args>
    explicit Error(fmt::format_string<Args...> fmt, Args&&... args): std::runtime_error(fmt::format(fmt, std::forward<Args>(args)...)) {}


    virtual ~Error() noexcept override = default;
};

class LogicError : public Error {
public:
    using Error::Error;
};

class RuntimeError : public Error {
public:
    using Error::Error;
};
}