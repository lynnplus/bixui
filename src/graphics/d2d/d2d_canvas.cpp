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

#include "d2d_canvas.h"

#include "bixlib/utils/fmt_bix.h"

#include "convert-inl.h"
#include "pen.h"
#include "text_format.h"

namespace bix {

using namespace std;

inline void throwIfD2DFailed(HRESULT hr, const string& msg) {
    if (hr != S_OK) { throw runtime_error(fmt::format("D2D:{}, HRESULT:{}", msg, hr)); }
}

D2DWindowTarget::D2DWindowTarget(DHwndRenderTargetPtr renderTarget, Direct2DEngine* engine)
    : mTarget(std::move(renderTarget))
    , mSafeScopeId(reinterpret_cast<uintptr_t>(mTarget.get())) {

    mWriteFactory = engine->writeFactory();
}

void D2DWindowTarget::beginDraw() {
    mTarget->BeginDraw();
}

DrawResult D2DWindowTarget::endDraw() {
    HRESULT hr = mTarget->EndDraw();
    if (hr == S_OK) { return DrawResult::Success; }
    if (hr == D2DERR_RECREATE_TARGET) { return DrawResult::RecreateCanvas; }
    fmt::println("Direct2DWindowTarget EndDraw fail: {}", hr);
    return DrawResult::Error;
}

void D2DWindowTarget::setTransform(const Transform& transform) {
    mTarget->SetTransform(convert_to_DMatrix(transform));
}

void D2DWindowTarget::resize(const UISize& size) {
    // Changes the size of the render target to the specified pixel size.
    auto hr = mTarget->Resize({numeric_cast<UINT32>(size.width), numeric_cast<UINT32>(size.height)});
    throwIfD2DFailed(hr, "resize error");
}

void D2DWindowTarget::clear(const Color& c) {
    mTarget->Clear(convert_to_DColorF(c));
}

bool D2DWindowTarget::pushClip(const UIFlexRoundedRect& rect) {
    assert(rect.isValid());

    ClipHolder holder{false};

    auto shape = rect.shape();
    if (shape == ShapeType::Rectangle) {
        mTarget->PushAxisAlignedClip(convert_to_DRectF(rect), D2D1_ANTIALIAS_MODE_PER_PRIMITIVE);
        mClipStack.push(holder);
        return true;
    }

    holder.isLayerClip = true;
    ID2D1Geometry* geometricMask = nullptr;
    ID2D1Factory* factoryPtr = nullptr;
    mTarget->GetFactory(&factoryPtr);

    if (shape == ShapeType::RoundedRectangle || shape == ShapeType::Ellipse) {

        D2D1_ROUNDED_RECT tmp(
            convert_to_DRectF(rect),
            static_cast<float>(rect.tl.radiusX),
            static_cast<float>(rect.tl.radiusY)
        );
        ID2D1RoundedRectangleGeometry* geometry = nullptr;
        auto hr = factoryPtr->CreateRoundedRectangleGeometry(tmp, &geometry);
        throwIfD2DFailed(hr, "create geometry error");
        geometricMask = geometry;
    }

    if (geometricMask == nullptr) { return false; }

    mTarget->PushLayer(
        D2D1_LAYER_PARAMETERS(
            D2D1::InfiniteRect(),
            geometricMask,
            D2D1_ANTIALIAS_MODE_PER_PRIMITIVE,
            D2D1::IdentityMatrix(),
            1.0
        ),
        nullptr
    );
    mClipStack.push(holder);
    geometricMask->Release();
    return true;
}

void D2DWindowTarget::popClip() {
    if (mClipStack.empty()) { throw std::runtime_error("pop clip fail,clip stack empty"); }
    auto [isLayerClip] = mClipStack.top();
    mClipStack.pop();
    if (isLayerClip) {
        mTarget->PopLayer();
    } else {
        mTarget->PopAxisAlignedClip();
    }
}

SizeF D2DWindowTarget::size() const noexcept {
    auto [width, height] = mTarget->GetSize();
    return {width, height};
}

void D2DWindowTarget::fillRectangle(const UIRect& rect, Brush& brush) {
    assert(brush.testCast(mSafeScopeId, D2DBasicBrush_CAST_ID));
    auto brushPtr = static_cast<D2DBasicBrush<>*>(&brush)->native();

    mTarget->FillRectangle(convert_to_DRectF(rect), brushPtr);
}

void D2DWindowTarget::drawRectangle(const UIRect& rect, Pen& pen) {
    assert(pen.testCast(mSafeScopeId, D2DPen_CAST_ID));
    auto penPtr = static_cast<D2DPen*>(&pen)->prepare();
    auto width = numeric_cast<float>(penPtr->strokeWidth());
    mTarget->DrawRectangle(convert_to_DRectF(rect), penPtr->brush(), width, penPtr->strokeStyle());
}

void D2DWindowTarget::drawRoundRect(const UIRect& rect, int radiusX, int radiusY, Pen& pen) {
    assert(pen.testCast(mSafeScopeId, D2DPen_CAST_ID));
    auto penPtr = static_cast<D2DPen*>(&pen)->prepare();
    auto width = numeric_cast<float>(penPtr->strokeWidth());
    mTarget->DrawRoundedRectangle(
        convert_to_DRoundRect(rect, radiusX, radiusY),
        penPtr->brush(),
        width,
        penPtr->strokeStyle()
    );
}

void D2DWindowTarget::drawEllipse(const UIEllipse& ellipse, Pen& pen) {
    assert(pen.testCast(mSafeScopeId, D2DPen_CAST_ID));
    auto penPtr = static_cast<D2DPen*>(&pen)->prepare();
    auto width = numeric_cast<float>(penPtr->strokeWidth());
    mTarget->DrawEllipse(convert_to_Ellipse(ellipse), penPtr->brush(), width, penPtr->strokeStyle());
}

void D2DWindowTarget::measureText(TextPaint& format, TextMetrics& metrics) {
    assert(format.testCast(mSafeScopeId, D2DTextFormat_CAST_ID));
    auto ptr = static_cast<D2DTextFormat*>(&format)->prepare();

    IDWriteTextLayout* layoutPtr = ptr->layout();

    DWRITE_TEXT_METRICS textMetrics{};
    auto hr = layoutPtr->GetMetrics(&textMetrics);
    throwIfD2DFailed(hr, "get text layout metrics fail");

    FLOAT minWidth = 0.f;
    hr = layoutPtr->DetermineMinWidth(&minWidth);
    throwIfD2DFailed(hr, "get text layout min-width fail");

    metrics.minWidth = static_cast<int>(ceil(minWidth));
    metrics.width = static_cast<int>(ceil(textMetrics.width));
    metrics.height = static_cast<int>(ceil(textMetrics.height));
    metrics.lineCount = static_cast<int>(textMetrics.lineCount);
}

void D2DWindowTarget::drawText(const UIPoint& origin, TextPaint& text, Pen& pen) {
    assert(pen.testCast(mSafeScopeId, D2DPen_CAST_ID));
    assert(text.testCast(mSafeScopeId, D2DTextFormat_CAST_ID));
    auto penPtr = static_cast<D2DPen*>(&pen)->prepare();
    auto textPtr = static_cast<D2DTextFormat*>(&text)->prepare();
    mTarget
        ->DrawTextLayout(convert_to_DPointF(origin), textPtr->layout(), penPtr->brush(), D2D1_DRAW_TEXT_OPTIONS_CLIP);
}

void D2DWindowTarget::drawLine(const UILine& line, Pen& pen) {
    assert(pen.testCast(mSafeScopeId, D2DPen_CAST_ID));
    auto penPtr = static_cast<D2DPen*>(&pen)->prepare();
    auto width = numeric_cast<float>(penPtr->strokeWidth());
    mTarget->DrawLine(
        convert_to_DPointF(line.p0),
        convert_to_DPointF(line.p1),
        penPtr->brush(),
        width,
        penPtr->strokeStyle()
    );
}

void D2DWindowTarget::drawLines(const vector<UILine>& lines, Pen& pen) {
    assert(pen.testCast(mSafeScopeId, D2DPen_CAST_ID));
    auto penPtr = static_cast<D2DPen*>(&pen)->prepare();
    auto width = numeric_cast<float>(penPtr->strokeWidth());

    for (const auto& line : lines) {
        mTarget->DrawLine(
            convert_to_DPointF(line.p0),
            convert_to_DPointF(line.p1),
            penPtr->brush(),
            width,
            penPtr->strokeStyle()
        );
    }
}

ColorBrushPtr D2DWindowTarget::createColorBrush(const Color& color) {
    ID2D1SolidColorBrush* brushPtr = nullptr;
    auto hr = mTarget->CreateSolidColorBrush(convert_to_DColorF(color), &brushPtr);
    throwIfD2DFailed(hr, "create color-brush fail");
    return std::make_unique<D2DSolidColorBrush>(DSolidColorBrushPtr(brushPtr), mTarget.get(), mSafeScopeId);
}

PenPtr D2DWindowTarget::createPen(const Color& color) {
    ID2D1SolidColorBrush* brushPtr = nullptr;
    auto hr = mTarget->CreateSolidColorBrush(convert_to_DColorF(color), &brushPtr);
    throwIfD2DFailed(hr, "create pen fail");
    return std::make_unique<D2DPen>(DSolidColorBrushPtr(brushPtr), color, mSafeScopeId);
}

TextPaintPtr D2DWindowTarget::createTextPaint() {
    return make_unique<D2DTextFormat>(mWriteFactory, mSafeScopeId, 1);
}
} // namespace bix