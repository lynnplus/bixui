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
#include "bixlib/common.h"
#include "bixlib/export_macro.h"

namespace bix {
enum class Axis {
    XAxis,
    YAxis,
    ZAxis
};

class BIX_PUBLIC Transform {
public:
    enum TransformationType {
        None,
        Translate,
        Scale,
        Rotate,
        Shear,
        Project,
    };

    //| a c e |
    //| b d f |
    //| 0 0 1 |
    Transform() = default;

    Transform(float m11, float m12, float m13, float m21, float m22, float m23, float m31, float m32, float m33)
        : mMatrix{{m11, m12, m13}, {m21, m22, m23}, {m31, m32, m33}}, mDirty(Project) {}

    static Transform fromTranslate(float dx, float dy);
    static Transform fromScale(float sx, float sy);

    TransformationType type() const;

    Transform& translate(float dx, float dy);
    Transform& scale(float sx, float sy);
    Transform& shear(float sh, float sv);
    void reset();

    const float* data() const;

private:
    float mMatrix[3][3]{{1, 0, 0}, {0, 1, 0}, {0, 0, 1}};
    mutable TransformationType mType = None;
    mutable TransformationType mDirty = None;
};
} // namespace bix
