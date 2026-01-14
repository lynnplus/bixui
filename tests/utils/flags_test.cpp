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

#include <bixlib/utils/flags.h>

#include <gtest/gtest.h>

namespace {
enum class TestFlag : uint32_t {
    None = 0,
    Read = 1 << 0,  // 1
    Write = 1 << 1, // 2
    Exec = 1 << 2,  // 4

    All = 0x7 // 1 | 2 | 4 = 7
};

BIX_DECLARE_ENUM_FLAGS(TestFlag)

using TestFlags = bix::Flags<TestFlag>;
} // namespace

using namespace bix;

class FlagsTest : public ::testing::Test {};

TEST_F(FlagsTest, Construction) {
    TestFlags fs;
    EXPECT_EQ(fs.value(), TestFlags::ZERO);
    EXPECT_FALSE(fs.testAny());

    TestFlags f_list{TestFlag::Read, TestFlag::Write};
    EXPECT_EQ(f_list.value(), 3);
}

TEST_F(FlagsTest, MaskAndLogic) {
    Flags fs(TestFlag::Read | TestFlag::Write);

    EXPECT_TRUE(fs.testFlag(TestFlag::Read));
    EXPECT_FALSE(fs.testFlag(TestFlag::Exec));

    EXPECT_FALSE(fs.testFlags(TestFlag::All));

    fs.on(TestFlag::Exec);
    EXPECT_TRUE(fs.testFlags(TestFlag::All));

    TestFlags any_check(TestFlag::Read);
    EXPECT_TRUE(any_check.testAnyFlags(TestFlag::All));
}

TEST_F(FlagsTest, ToggleAndCount) {
    TestFlags fs(TestFlag::All);

    EXPECT_EQ(fs.count(), 3);

    fs.toggleAll<3>();
    EXPECT_EQ(fs.value(), 0);

    fs.toggleAll();
    EXPECT_EQ(fs.count(), TestFlags::MAX_SIZE);
}

TEST_F(FlagsTest, BitsetAndOperators) {
    TestFlags fs = TestFlag::Read | TestFlag::Exec; // 101 (binary)

    auto b = fs.bits<8>();
    EXPECT_EQ(b.to_string().back(), '1');
    EXPECT_TRUE(b.test(2));

    auto combined = fs & TestFlag::Read;
    EXPECT_EQ(combined.value(), static_cast<uint32_t>(TestFlag::Read));
}

TEST_F(FlagsTest, ZeroBehavior) {
    TestFlags empty;

    EXPECT_TRUE(empty.testFlags(TestFlag::None));

    TestFlags some(TestFlag::Read);
    EXPECT_FALSE(some.testFlags(TestFlag::None));
}