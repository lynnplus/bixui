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

#include "bixlib/utils/concepts.h"

#include <format>
#include <limits>
#include <stdexcept>
#include <utility>

namespace bix {
template <Arithmetic To, Arithmetic From>
constexpr void throw_cast_overflow(From value) {
    throw std::overflow_error(
        std::format("value: {},overflow casting from {} to {}.", value, typeid(From).name(), typeid(To).name()));
}

template <Arithmetic To, Arithmetic From>
constexpr void throw_cast_underflow(From value) {
    throw std::underflow_error(
        std::format("value: {},underflow casting from {} to {}.", value, typeid(From).name(), typeid(To).name()));
}

template <Arithmetic To, Arithmetic From>
constexpr To numeric_cast(From value) {
    using ToLimit = std::numeric_limits<To>;
    using FromLimit = std::numeric_limits<From>;

    if constexpr (std::is_same_v<From, To>) {
        return value;
    }

    static_assert(FromLimit::radix == 2);
    static_assert(ToLimit::radix == 2);

    if constexpr (ToLimit::is_integer && FromLimit::is_integer) {
        if (std::cmp_greater(value, ToLimit::max())) {
            throw_cast_overflow<To, From>(value);
        }
        if (std::cmp_less(value, ToLimit::lowest())) {
            throw_cast_underflow<To, From>(value);
        }
    } else {
        constexpr bool is_upper = ToLimit::digits >= FromLimit::digits;
        constexpr int digits = (is_upper ? FromLimit::digits : ToLimit::digits) - 1;
        constexpr From max = From(((1 << digits) - 1) * 2); // max-1,prevent overflow

        if (value > 0 && value > max) {
            throw_cast_overflow<To, From>(value);
        }
        if (value < 0 && value < max) {
            throw_cast_underflow<To, From>(value);
        }
    }
    return static_cast<To>(value);
}

template <FloatType T>
constexpr T neg_pow10(int n) {
    T result = 1;
    for (int i = 1; i <= n; ++i)
        result /= 10;
    return result;
}

template <FloatType T, int Precision = std::numeric_limits<T>::digits10 - 1>
requires(Precision > 0 && Precision <= std::numeric_limits<T>::digits10 - 1)
constexpr bool fuzzyCompareEqual(T p1, T p2) noexcept {
    constexpr T eps = neg_pow10<T>(Precision);
    return std::fabs(p1 - p2) <= eps;
}

template <FloatType T, int Precision = std::numeric_limits<T>::digits10 - 1>
requires(Precision > 0 && Precision <= std::numeric_limits<T>::digits10 - 1)
constexpr bool fuzzyEqualZero(T v) noexcept {
    constexpr T eps = neg_pow10<T>(Precision);
    return std::fabs(v) <= eps;
}
} // namespace bix