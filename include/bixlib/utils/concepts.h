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
#include <concepts>
#include <memory>
#include <type_traits>

namespace bix {

/**
 * Concept for arithmetic types.
 * Defines a type constraint for integral or floating-point types,
 * Used to enforce arithmetic operations in templates,
 * Ensures type compatibility with arithmetic operations.
 * @code
 * //Example usage:
 * template<Arithmetic T> void process(T value);
 * @endcode
 */
template <typename T>
concept Arithmetic = std::is_arithmetic_v<T>;

template <typename T>
concept Real = std::is_arithmetic_v<T> && std::is_signed_v<T>;

template <typename Dp, typename Bp>
concept DerivedFrom = std::derived_from<Dp, Bp>;

template <typename T>
concept FloatType = std::is_floating_point_v<T>;

template <class T>
concept EnumType = std::is_enum_v<T>;

namespace traits {
template <typename T>
struct IsUniquePtr : std::false_type {};

template <typename T>
struct IsUniquePtr<std::unique_ptr<T>> : std::true_type {};

template <typename T>
concept UniquePtrType = IsUniquePtr<std::remove_cvref_t<T>>::value;

template <typename... Args>
struct FirstArg {
    using type = void;
};

template <typename T, typename... Args>
struct FirstArg<T, Args...> {
    using type = T;
};

template <typename... Args>
using FirstArgT = typename FirstArg<Args...>::type;
} // namespace traits

} // namespace bix