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
#include "bixlib/geometry/size.h"

#include <string>

namespace bix {
// enum class Rotation


enum class DisplayUnit {
    Invalid,
    DP,
    PX,
};

/**
 * Provides information about the size and density of a logical display.
 */
class BIX_PUBLIC Display {
public:
    virtual ~Display() = default;

    [[nodiscard]]
    virtual const std::string& deviceName() const noexcept =0;
    [[nodiscard]]
    virtual bool isValid() const noexcept =0;
    /**
     * Gets the size(native resolution) of the display in pixels.
     * @return
     */
    [[nodiscard]]
    virtual const UISize& resolution() const noexcept =0;
    /**
     *
     * @return logical resolution
     */
    [[nodiscard]]
    virtual const UISize& size() const noexcept =0;

    [[nodiscard]]
    virtual int refreshRate() const noexcept =0;

    [[nodiscard]]
    virtual int dpi() const noexcept =0;

    [[nodiscard]]
    virtual int baseDPI() const noexcept =0;

    [[nodiscard]]
    virtual bool isDefault() const noexcept =0;
    // virtual int rotation();
};

using DisplayPtr = std::shared_ptr<Display>;
using DisplayCRef = const std::shared_ptr<Display>&;


class DisplayStrategy {
public:
    virtual ~DisplayStrategy() = default;

    //px = dp × DPI / BASE
    //dp = px * BASE / DPI
    virtual float calculateDensity(const Display& display) =0;
};

using DisplayStrategyPtr = std::shared_ptr<DisplayStrategy>;
using DisplayStrategyCRef = const std::shared_ptr<DisplayStrategy>&;
} // bix
