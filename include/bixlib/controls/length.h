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
        AutoMode,
        FixedMode,
        PercentageMode,
        FillMode
    };

    UILength() = default;

    constexpr explicit UILength(int v) {
        mMode = FixedMode;
        if (v < 0) {
            v = 0;
        }
        mValue = v;
    }

    constexpr explicit UILength(Mode m) { mMode = m; }

    constexpr explicit UILength(float v) {
        mMode = PercentageMode;
        if (v > 1.f) {
            v = 1.f;
        }
        if (v < 0.f) {
            v = 0.f;
        }
        mValue = numeric_cast<int>(v * 100.f);
    }

    Mode mode() const { return mMode; }

    bool isAuto() const { return mMode == AutoMode; }

    bool isFixed() const { return mMode == FixedMode; }

    bool isRelative() const { return mMode == PercentageMode || mMode == FillMode; }

    int fixed() const {
        if (mMode != FixedMode) {
            return 0;
        }
        return mValue;
    }

    int get(int v) const noexcept{
        switch (mMode) {
        case PercentageMode:
            return v * mValue / 100;
        case FillMode:
            return v;
        case AutoMode:
            return -1;
        default:
            return mValue;
        }
    }

private:
    int mValue = 0;
    Mode mMode = AutoMode;
};

constexpr inline UILength FullParent{UILength::FillMode};
constexpr inline UILength AutoContent{UILength::AutoMode};

struct SpecSize {
    UILength width{};
    UILength height{};

    UISize get(const UISize& src) const noexcept { return {width.get(src.width), height.get(src.height)}; }
};
} // namespace bix
