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

// #include <type_traits>

namespace bix {
// // ReSharper disable CppTemplateParameterNeverUsed
// template<typename From, typename To, typename Enable= void>
// struct has_converter : std::false_type {};
//
// template<typename From, typename To>
// struct has_converter<From, To, std::enable_if_t<
//     std::is_same_v<decltype(convert_as(std::declval<const From&>())), To>
// >> :std::true_type {};
//
//
// template<typename From, typename To>
// inline constexpr bool has_converter_v =  has_converter<From, To>::value;
//
// template<typename From, typename To, typename = has_converter<From, To>>
// To convert_if_possible(const From& src){
//     return convert_as(src);
// }
//
//
// #define BIX_IMPLICITLY_CONVERTIBLE(name) \
// template <typename To> \
// requires has_converter_v<name,To> \
// constexpr explicit operator To() const { \
// return convert_if_possible<name,To>(*this); \
// }
}