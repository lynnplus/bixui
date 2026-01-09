/*
 * Copyright (c) 2025-2026 Lynn <lynnplus90@gmail.com>
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
#include <bixlib/geometry/point.h>
#include <bixlib/geometry/rect.h>
#include <bixlib/geometry/size.h>

#include <string>

namespace bix {

/**
 * Represents a logical display area, which could be a physical monitor or a virtual display.
 * This class provides properties for both physical hardware attributes and logical
 * layout metrics used by the UI framework.
 */
class BIX_PUBLIC Screen {
public:
    virtual ~Screen() = default;
    /**
     * Returns a unique identifier for the screen.
     *
     * The ID is intended to be persistent across application sessions to
     * identify the same physical or virtual device even after rebooting.
     * @return A string containing the platform-specific unique identifier.
     * @note The format of the ID depends on the backend (e.g., serial hash or UUID).
     */
    virtual std::string id() const noexcept = 0;
    /**
     * Returns the user-friendly name of the screen.
     *
     * @return A string representing the display name (e.g., "\\.\DISPLAY1").
     */
    virtual std::string name() const noexcept = 0;
    /**
     * Checks if the screen is currently active and available for rendering.
     *
     * This status indicates whether the screen object is still linked to an active
     * display device in the operating system.
     * @return True if the display is connected and valid; false otherwise.
     * @warning Querying other properties on an unavailable screen may return stale
     * cached data from its last known state.
     */
    virtual bool isAvailable() const noexcept = 0;
    /**
     * Returns true if this is the primary screen of the operating system.
     *
     * The primary screen usually hosts the main taskbar and desktop icons.
     * @return True if this is the primary screen, false otherwise.
     */
    virtual bool isPrimary() const noexcept = 0;
    /**
     * Returns the top-left corner of the screen in the global virtual desktop coordinates.
     *
     * This coordinate system allows windows to be positioned across multiple monitors.
     * @return A Point object representing the (x, y) coordinates.
     */
    virtual Point position() const noexcept = 0;
    /**
     * Returns the actual hardware resolution in pixels.
     *
     * This represents the raw pixel count of the display panel, independent of
     * any OS-level scaling or DPI settings.
     * @return A Size object representing the width and height in physical pixels.
     */
    virtual Size physicalSize() const noexcept = 0;
    /**
     * Returns the logical size of the screen in density-independent units.
     * @return A Size object representing the width and height in logical units.
     * @note Use this value for UI layout and widget positioning.
     */
    virtual Size size() const noexcept = 0;
    /**
     * Returns the usable area of the screen excluding system-reserved bars.
     *
     * This Rect defines the space where application windows can be placed without
     * being obstructed by the Taskbar, Dock, or Status Bar.
     * @return A Rect object representing the work area bounds in logical coordinates.
     * @note The origin (x, y) may be non-zero if system bars are at the top or left.
     */
    virtual Rect workArea() const noexcept = 0;
    /**
     * Returns the scaling factor between logical units and physical pixels.
     *
     * A value of 1.0 represents standard density (96 DPI on Windows), while 2.0
     * represents High-DPI or Retina displays.
     * @return The floating-point scale factor (e.g., 1.5 for 150% scaling).
     */
    virtual float scaleFactor() const noexcept = 0;
    /**
     * Returns the vertical refresh rate of the screen in Hertz (Hz).
     * @return The refresh rate in Hz. Returns 0 if the rate is unknown.
     */
    virtual int refreshRate() const noexcept = 0;
    /**
     * Returns the current dots per inch (DPI) of the screen.
     *
     * This is the effective DPI reported by the OS, affected by user scaling settings.
     * @return The dots per inch value.
     */
    virtual int dpi() const noexcept = 0;
    /**
     * Returns the baseline DPI for the current platform.
     *
     * This is the reference value where the scale factor is 1.0 (e.g., 96 on Windows).
     * @return The standard DPI value for the underlying OS.
     */
    virtual int standardDPI() const noexcept = 0;
    /**
     * Returns the current logical rotation of the screen.
     *
     * This reflects how the OS has oriented the display content.
     * @return The rotation angle in degrees (typically 0, 90, 180, or 270).
     */
    virtual int rotation() const noexcept = 0;
};

/**
 * Shared pointer to a Screen object.
 * * This is the standard way to hold and pass screen references within the framework
 * to ensure memory safety across different threads and components.
 */
using ScreenPtr = std::shared_ptr<Screen>;

} // namespace bix
