/*
 * Copyright (c) 2026 Lynn <lynnplus90@gmail.com>
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

#include <bixlib/graphics/colors.h>

#include <gtest/gtest.h>

using namespace bix;
using namespace bix::literals;

/**
 * Test basic construction and component access.
 */
TEST(ColorTest, Construction) {
    Color c(255, 128, 0, 200);
    EXPECT_EQ(c.red(), 255);
    EXPECT_EQ(c.green(), 128);
    EXPECT_EQ(c.blue(), 0);
    EXPECT_EQ(c.alpha(), 200);
    EXPECT_TRUE(c.isValid());
}

/**
 * Test hexadecimal string parsing.
 */
TEST(ColorTest, FromHexString) {
    // 6-digit with #
    EXPECT_EQ(Color::fromHexString("#FF0000"), Color(255, 0, 0));

    // 3-digit shorthand
    EXPECT_EQ(Color::fromHexString("#F00"), Color(255, 0, 0));

    // 8-digit with alpha
    EXPECT_EQ(Color::fromHexString("#00FF0080"), Color(0, 255, 0, 128));

    // No # prefix
    EXPECT_EQ(Color::fromHexString("0000FF"), Color(0, 0, 255));

    EXPECT_EQ(Color::fromHexString("ff0000"), Color(255, 0, 0));
    EXPECT_EQ(Color::fromHexString("Ff0000"), Color(255, 0, 0));

    // Invalid formats
    EXPECT_FALSE(Color::fromHexString("").isValid());
    EXPECT_FALSE(Color::fromHexString("mmmnnn").isValid());
    EXPECT_FALSE(Color::fromHexString("invalid").isValid());
    EXPECT_FALSE(Color::fromHexString("#GG0000").isValid());
    EXPECT_FALSE(Color::fromHexString("#12345").isValid()); // Wrong length
}

/**
 * Test the newly added integer literals.
 * This ensures our bit-shifting logic is correct.
 */
TEST(ColorTest, IntegerLiterals) {
    // _rgb (24-bit)
    Color c1 = 0xFF5733_rgb;
    EXPECT_EQ(c1.red(), 0xFF);
    EXPECT_EQ(c1.green(), 0x57);
    EXPECT_EQ(c1.blue(), 0x33);
    EXPECT_EQ(c1.alpha(), 255);

    // _rgba (32-bit)
    Color c2 = 0x11223344_rgba;
    EXPECT_EQ(c2.red(), 0x11);
    EXPECT_EQ(c2.green(), 0x22);
    EXPECT_EQ(c2.blue(), 0x33);
    EXPECT_EQ(c2.alpha(), 0x44);

    // Edge case: short value in _rgba
    // Should be 0x000000FF (Black with 255 alpha)
    EXPECT_EQ(0x000000FF_rgba, Color(0, 0, 0, 255));
}

/**
 * Test string literals and shared bix::literals namespace.
 */
TEST(ColorTest, StringLiterals) {
    auto c = "#ABC"_rgba;
    EXPECT_EQ(c, Color(170, 187, 204, 255)); // #AABBCC
}

/**
 * Test Linear Interpolation (lerp) and float conversion.
 */
TEST(ColorTest, Interpolation) {
    Color start(0, 0, 0, 0);
    Color end(255, 255, 255, 255);
    // Midpoint
    Color mid = Color::lerp(start, end, 0.5f);
    EXPECT_EQ(mid.red(), 128); // round(255 * 0.5) = 128

    // Start/End clamp
    EXPECT_EQ(Color::lerp(start, end, -1.0f), start);
    EXPECT_EQ(Color::lerp(start, end, 2.0f), end);

    // AlphaF testing
    Color opaque = Color(0, 0, 0).alphaF(0.5f);
    EXPECT_NEAR(opaque.alphaF(), 0.5f, 0.01f);
}

TEST(ColorTest, LerpLogic) {
    Color white(255, 255, 255, 255);
    Color black(0, 0, 0, 255);

    Color gray = Color::lerp(black, white, 0.5f);
    EXPECT_EQ(gray.red(), 128);
    EXPECT_EQ(gray.green(), 128);
    EXPECT_EQ(gray.blue(), 128);

    EXPECT_EQ(Color::lerp(black, white, -1.0f), black);
    EXPECT_EQ(Color::lerp(black, white, 2.0f), white);

    Color transparent(255, 0, 0, 0);
    Color opaque(255, 0, 0, 254);
    Color half = Color::lerp(transparent, opaque, 0.5f);
    EXPECT_EQ(half.alpha(), 127);
}

/**
 * Ensure constants in bix::colors are correct.
 */
TEST(ColorTest, Constants) {
    EXPECT_EQ(colors::Red.red(), 255);
    EXPECT_EQ(colors::Transparent.alpha(), 0);
}