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
#include "bixlib/animation/curve.h"

#include <chrono>

namespace bix {

/**
 * this is anim
 */
class BIX_PUBLIC Animator {
public:
    using Duration = std::chrono::milliseconds;

    virtual ~Animator() = default;

    virtual void start() = 0;
    virtual void cancel() = 0;
    virtual void end() = 0;
    virtual void pause() = 0;
    virtual void resume() = 0;

    virtual bool isStarted() = 0;
    virtual bool isRunning() = 0;
    virtual bool isPaused() = 0;

    virtual Duration duration() = 0;
    virtual Duration totalDuration() = 0;
    virtual Duration startDelay() = 0;

    virtual void setStartDelay(Duration startDelay);

    virtual void setDuration(Duration duration) = 0;
    virtual void setCurve(AnimCurvePtr curve) = 0;
};

/**
 * Smart pointer type for managing Animator objects
 *
 * Provides automatic memory management for Animator instances using
 * std::shared_ptr, enabling shared ownership and reference counting.
 * This is the preferred way to handle Animator objects in the codebase.
 *
 * @note Using shared_ptr allows multiple components to share ownership
 * of the same Animator instance, ensuring it remains alive as long as
 * any reference exists.
 */
// TODO ??? doc
using AnimatorPtr = std::shared_ptr<Animator>;

class BIX_PUBLIC AnimatorListener {
public:
};

class BaseAnimator : public Animator {
public:
};

} // namespace bix
