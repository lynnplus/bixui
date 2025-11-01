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

namespace bix {
class BIX_PUBLIC UILength {
public:
    enum Mode {
        FixedMode,
        PercentageMode,
        NegativeMode, //full auto
    };

    enum Preset {
        AutoLength = -1,
        FullLength = -2,
    };

    UILength() = default;

    constexpr explicit UILength(int v) {
        if (v == AutoLength || v == FullLength) {
            mMode = NegativeMode;
        } else {
            mMode = FixedMode;
            if (v <= 0) {
                v = 0;
            }
        }
        mValue.intValue = v;
    }

    constexpr explicit UILength(float v) {
        if (v <= 0.00001) {
            mMode = FixedMode;
            mValue.intValue = 0;
        } else {
            mMode = PercentageMode;
            mValue.floatValue = v;
        }
    }

    Mode mode() const { return mMode; }

    int fixed() const {
        if (mMode != FixedMode) {
            return 0;
        }
        return mValue.intValue;
    }

private:
    union {
        int intValue;
        float floatValue;
    } mValue{AutoLength};

    Mode mMode = NegativeMode;
};

constexpr inline UILength FullParent{UILength::FullLength};
constexpr inline UILength AutoParent{UILength::AutoLength};

struct SpecSize {
    UILength width{};
    UILength height{};
};
}
