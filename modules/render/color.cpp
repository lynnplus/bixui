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

#include <stdexcept>
#include <bixlib/render/color.h>

namespace bix {

inline int h2i(char hex) {
    if (hex >= '0' && hex <= '9')
        return hex - '0';
    if (hex >= 'a' && hex <= 'f')
        return hex - 'a' + 10;
    if (hex >= 'A' && hex <= 'F')
        return hex - 'A' + 10;
    return -1;
}


Color Color::fromHexString(const std::string& str) {
    if (str.size() < 7 || str[0] != '#') {
        throw std::invalid_argument("Invalid hex color format");
    }
    auto hex2int = [](const char* s) {
        return h2i(s[0]) << 4 | h2i(s[1]);
    };

    int a = 255;
    int r = hex2int(str.c_str() + 1);
    int g = hex2int(str.c_str() + 3);
    int b = hex2int(str.c_str() + 5);
    if (str.length() == 9) {
        a = hex2int(str.c_str() + 7);
    }
    return {r, g, b, a};
}


}
