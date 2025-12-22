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
#include "engine.h"

namespace bix {


enum class ClipType {
    AlignedClip,
    Layer
};

class ClipHolder {
public:
    bool isLayerClip = false;
};

// https://learn.microsoft.com/zh-cn/windows/win32/learnwin32/dpi-and-device-independent-pixels?redirectedfrom=MSDN
class D2DWindowTarget : public Canvas {
public:
    D2DWindowTarget(DHwndRenderTargetPtr renderTarget,Direct2DEngine* engine);

    [[nodiscard]] ColorBrushPtr createColorBrush(const Color& color) override;
    [[nodiscard]] PenPtr createPen(const Color& color) override;
    [[nodiscard]] TextPaintPtr createTextPaint() override;

    void beginDraw() override;
    DrawResult endDraw() override;
    void setTransform(const Transform& transform) override;
    void resize(const UISize& size) override;
    void clear(const Color& c) override;
    bool pushClip(const UIFlexRoundedRect& rect) override;
    void popClip() override;
    SizeF size() const noexcept override;
    void fillRectangle(const UIRect& rect, Brush& brush) override;
    void drawRectangle(const UIRect& rect, Pen& pen) override;
    void drawRoundRect(const UIRect& rect, int radiusX, int radiusY, Pen& pen) override;
    void drawEllipse(const UIEllipse& ellipse, Pen& pen) override;
    void measureText(TextPaint& format, TextMetrics& metrics) override;
    void drawText(const UIPoint& origin, TextPaint& text, Pen& pen) override;
    void drawLine(const UILine& line, Pen& pen) override;
    void drawLines(const std::vector<UILine>& lines, Pen& pen) override;

protected:
    DHwndRenderTargetPtr mTarget = nullptr;
    const uintptr_t mSafeScopeId;

    IDWriteFactory* mWriteFactory = nullptr;
    DLayerPtr mCurrentLayer = nullptr;

private:
    std::stack<ClipHolder> mClipStack;
};

} // namespace bix