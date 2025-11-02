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

#include "bixlib/render/transform.h"

#include <valarray>

#include "bixlib/export_macro.h"

namespace bix {


bool fuzzyIsZero(float v) { return std::abs(v) <= 0.00001f; }

Transform Transform::fromTranslate(float dx, float dy) {
    Transform tm(1, 0, 0, 0, 1, 0, dx, dy, 1);
    tm.mDirty = None;
    if (fuzzyIsZero(dx) && fuzzyIsZero(dy)) {
        tm.mType = None;
    } else {
        tm.mType = Translate;
    }
    return tm;
}

Transform Transform::fromScale(float sx, float sy) {
    Transform tm(sx, 0, 0, 0, sy, 0, 0, 0, 1);
    tm.mDirty = None;
    if (sx == 1.f && sy == 1.f) {
        tm.mType = None;
    } else {
        tm.mType = Scale;
    }
    return tm;
}

Transform::TransformationType Transform::type() const {
    if (mDirty == None || mDirty < mType) {
        return mType;
    }

    switch (mDirty) {
    case Project:
        if (!fuzzyIsZero(mMatrix[0][2]) || !fuzzyIsZero(mMatrix[1][2]) || !fuzzyIsZero(mMatrix[2][2] - 1)) {
            mType = Project;
            break;
        }
        [[fallthrough]];
    case Shear:
    case Rotate:
        if (!fuzzyIsZero(mMatrix[0][1]) || !fuzzyIsZero(mMatrix[1][0])) {
            const float dot = mMatrix[0][0] * mMatrix[1][0] + mMatrix[0][1] * mMatrix[1][1];
            if (fuzzyIsZero(dot))
                mType = Rotate;
            else
                mType = Shear;
            break;
        }
        [[fallthrough]];
    case Scale:
        if (!fuzzyIsZero(mMatrix[0][0] - 1) || !fuzzyIsZero(mMatrix[1][1] - 1)) {
            mType = Scale;
            break;
        }
        [[fallthrough]];
    case Translate:
        if (!fuzzyIsZero(mMatrix[2][0]) || !fuzzyIsZero(mMatrix[2][1])) {
            mType = Translate;
            break;
        }
        [[fallthrough]];
    case None:
        mType = None;
        break;
    }

    mDirty = None;
    return mType;
}

Transform& Transform::translate(float dx, float dy) {
    if (dx == 0.0f && dy == 0.0f) {
        return *this;
    }

    switch (type()) {
    case None:
        mMatrix[2][0] = dx;
        mMatrix[2][1] = dy;
        break;
    case Translate:
        mMatrix[2][0] += dx;
        mMatrix[2][1] += dy;
        break;
    case Scale:
        mMatrix[2][0] += dx * mMatrix[0][0];
        mMatrix[2][1] += dy * mMatrix[1][1];
        break;
    case Project:
        mMatrix[2][2] += dx * mMatrix[0][2] + dy * mMatrix[1][2];
        [[fallthrough]];
    case Shear:
    case Rotate:
        mMatrix[2][0] += dx * mMatrix[0][0] + dy * mMatrix[1][0];
        mMatrix[2][1] += dy * mMatrix[1][1] + dx * mMatrix[0][1];
        break;
    }
    if (mDirty < Translate)
        mDirty = Translate;
    return *this;
}

Transform& Transform::scale(float sx, float sy) {
    if (sx == 1.f && sy == 1.f) {
        return *this;
    }
    switch (type()) {
    case None:
    case Translate:
        mMatrix[0][0] = sx;
        mMatrix[1][1] = sy;
        break;
    case Project:
        mMatrix[0][2] *= sx;
        mMatrix[1][2] *= sy;
        [[fallthrough]];
    case Rotate:
    case Shear:
        mMatrix[0][1] *= sx;
        mMatrix[1][0] *= sy;
        [[fallthrough]];
    case Scale:
        mMatrix[0][0] *= sx;
        mMatrix[1][1] *= sy;
        break;
    }
    if (mDirty < Scale)
        mDirty = Scale;
    return *this;
}

Transform& Transform::shear(float sh, float sv) {
    //TODO not implemented
    BIX_UNUSED(sh)
    BIX_UNUSED(sv)
    return *this;
}

void Transform::reset() { *this = Transform(); }

const float* Transform::data() const { return &mMatrix[0][0]; }
} // namespace bix
