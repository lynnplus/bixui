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

#include "engine.h"

#include <fmt/base.h>

#include "convert-inl.h"
#include "d2d_canvas.h"

namespace bix {

using namespace std;

Direct2DEngine::~Direct2DEngine() {
    fmt::println("Direct2DEngine::~Direct2DEngine");
}

void Direct2DEngine::shutdown() noexcept {
    mDWriteFactory = nullptr;
    mD2DFactory = nullptr;
}

RenderEngine::Type Direct2DEngine::type() const noexcept {
    return Type::Direct2D;
}

CanvasPtr Direct2DEngine::createCanvas(const Window& w) {

    auto wnd = reinterpret_cast<HWND>(w.nativeHandle());

    RECT rc;
    GetClientRect(wnd, &rc); // pixel unit

    D2D1_SIZE_U size = D2D1::SizeU(static_cast<UINT32>(rc.right - rc.left), static_cast<UINT32>(rc.bottom - rc.top));

    fmt::println("createCanvas w:{} h:{}", size.width, size.height);
    auto rtProps = D2D1::HwndRenderTargetProperties(wnd, size);
    auto props = D2D1::RenderTargetProperties();
    props.dpiX = 96;
    props.dpiY = 96; // 1dp=1px
    ID2D1HwndRenderTarget* target = nullptr;
    // Create a Direct2D render target. use pixel size from window client
    auto hr = mD2DFactory->CreateHwndRenderTarget(props, rtProps, &target);
    if (hr != S_OK) {
        fmt::println("Failed to create render target");
        return nullptr;
    }
    return make_unique<D2DWindowTarget>(DHwndRenderTargetPtr(target), this);
}

IDWriteTextFormat* Direct2DEngine::createFont() {

    IDWriteTextFormat* format = nullptr;

    // virtual HRESULT STDMETHODCALLTYPE CreateTextFormat(
    //     const WCHAR *family_name,
    //     IDWriteFontCollection *collection,
    //     DWRITE_FONT_WEIGHT weight,
    //     DWRITE_FONT_STYLE style,
    //     DWRITE_FONT_STRETCH stretch,
    //     FLOAT size,
    //     const WCHAR *locale,
    //     IDWriteTextFormat **format) = 0;
    auto hr = mDWriteFactory->CreateTextFormat(
        L"Gabriola", // Font family name.
        nullptr,     // Font collection (NULL sets it to use the system font collection).
        DWRITE_FONT_WEIGHT_REGULAR,
        DWRITE_FONT_STYLE_NORMAL,
        DWRITE_FONT_STRETCH_NORMAL,
        12.0f,
        L"en-us",
        &format
    );
    hr = format->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
    hr = format->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
    format->SetFlowDirection(DWRITE_FLOW_DIRECTION_TOP_TO_BOTTOM);
    format->SetIncrementalTabStop(0.1f);
    format->SetReadingDirection(DWRITE_READING_DIRECTION_LEFT_TO_RIGHT);
    // format->SetWordWrapping()

    // IDWriteFontFace* fontFace=nullptr;

    // mDWriteFactory->CreateFontFace()

    if (hr) {}

    return format;
}

IDWriteFactory* Direct2DEngine::writeFactory() const noexcept {
    return mDWriteFactory.get();
}

Direct2DEngine::Direct2DEngine() {
    // Create a Direct2D factory.

    ID2D1Factory* factory = nullptr;

#ifndef NDEBUG // debug mode
    constexpr D2D1_FACTORY_OPTIONS opt(D2D1_DEBUG_LEVEL_INFORMATION);
    HRESULT hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, opt, &factory);
#else
    HRESULT hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &factory);
#endif

    if (hr == S_OK) { mD2DFactory = DFactorPtr(factory); }

    IDWriteFactory* wfactory = nullptr;
    hr = DWriteCreateFactory(
        DWRITE_FACTORY_TYPE_SHARED,
        __uuidof(IDWriteFactory),
        reinterpret_cast<IUnknown**>(&wfactory)
    );
    if (hr == S_OK) { mDWriteFactory = DWriteFactoryPtr(wfactory); }
}
} // namespace bix
