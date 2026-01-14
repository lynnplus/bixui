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
#include <bixlib/utils/numeric.h>

#include <gtest/gtest.h>

using namespace bix::math;

static_assert(numeric_cast<int>(5.0f) == 5, "numeric_cast should work at compile time");
static_assert(exactlyEqual(numeric_cast<float>(5), 5.f), "numeric_cast should work at compile time");
static_assert(numeric_cast<uint8_t>(5) == 5, "numeric_cast should work at compile time");

TEST(FuzzyCompareTest, FuzzyEqual) {

    // 1. Test float with default epsilon (1e-5f)
    EXPECT_TRUE(fuzzyEqual(1.0f, 1.000001f)); // Diff = 1e-6 (Equal)
    EXPECT_FALSE(fuzzyEqual(1.0f, 1.0001f));  // Diff = 1e-4 (Not equal)

    // 2. Test double with default epsilon (1e-10)
    EXPECT_TRUE(fuzzyEqual(1.0, 1.00000000001)); // Diff = 1e-11 (Equal)
    EXPECT_FALSE(fuzzyEqual(1.0, 1.00000001));   // Diff = 1e-8 (Not equal)

    // 3. Test with custom epsilon
    EXPECT_TRUE(fuzzyEqual(1.0f, 1.1f, 0.2f));
    EXPECT_FALSE(fuzzyEqual(1.0f, 1.1f, 0.05f));

    // 4. Test symmetry and negative values
    EXPECT_TRUE(fuzzyEqual(-1.0, -1.00000000001));
    EXPECT_TRUE(fuzzyEqual(1.0, 1.0));
    EXPECT_FALSE(fuzzyEqual(1.0, -1.0));

    // Difference exactly equals epsilon
    EXPECT_TRUE(fuzzyEqual(1.0f, 1.00001f, 1.1e-5f));
    EXPECT_TRUE(fuzzyEqual(1.0f, 0.999999f, 1e-5f));

    EXPECT_TRUE(fuzzyEqual(0.0f, 1e-6f, 1e-5f));
    EXPECT_FALSE(fuzzyEqual(0.0f, 1e-4f, 1e-5f));
    // Test -0.0 and 0.0 equality
    EXPECT_TRUE(fuzzyEqual(0.0f, -0.0f));

    // Special values
    float inf = std::numeric_limits<float>::infinity();
    float nan = std::numeric_limits<float>::quiet_NaN();

    EXPECT_TRUE(fuzzyEqual(inf, inf));   // Inf equals itself
    EXPECT_FALSE(fuzzyEqual(inf, -inf)); // Different signs
    EXPECT_FALSE(fuzzyEqual(nan, nan));  // NaN never equals anything, even itself
    EXPECT_FALSE(fuzzyEqual(1.0f, nan));
}

TEST(FuzzyCompareTest, FuzzyIsZero) {
    // 1. Test float boundaries
    EXPECT_TRUE(fuzzyIsZero(0.000001f)); // < 1e-5f
    EXPECT_FALSE(fuzzyIsZero(0.0001f));  // > 1e-5f

    // 2. Test double boundaries
    EXPECT_TRUE(fuzzyIsZero(1e-11)); // < 1e-10
    EXPECT_FALSE(fuzzyIsZero(1e-9)); // > 1e-10

    // 3. Test Negative Zero (-0.0)
    // In UI frameworks, calculations often result in -0.0
    EXPECT_TRUE(fuzzyIsZero(-0.0f));
    EXPECT_TRUE(fuzzyIsZero(-0.0));

    // 4. Test negative small values
    EXPECT_TRUE(fuzzyIsZero(-0.000001f));
    EXPECT_FALSE(fuzzyIsZero(-0.1f));

    // 5. Test with custom epsilon
    EXPECT_TRUE(fuzzyIsZero(-0.01f, 0.01f));
    EXPECT_FALSE(fuzzyIsZero(-0.001, 0.0001));
    EXPECT_FALSE(fuzzyIsZero(0.001, 0.0001));

    // 6. Boundary values (Value == Epsilon)
    EXPECT_TRUE(fuzzyIsZero(1e-5f, 1e-5f));
    EXPECT_TRUE(fuzzyIsZero(-1e-5f, 1e-5f));

    // 7. Extremely small values (Subnormal)
    EXPECT_TRUE(fuzzyIsZero(std::numeric_limits<double>::denorm_min()));

    // 8. Infinity and NaN should never be zero
    float inf = std::numeric_limits<float>::infinity();
    float nan = std::numeric_limits<float>::quiet_NaN();
    EXPECT_FALSE(fuzzyIsZero(inf));
    EXPECT_FALSE(fuzzyIsZero(nan));

    // 9. Precision distinction (float vs double default eps)
    // 1e-7 is zero for float (eps 1e-5), but NOT zero for double (eps 1e-10)
    EXPECT_TRUE(fuzzyIsZero(1e-7f));
    EXPECT_FALSE(fuzzyIsZero(1e-7));
}

class NumericCastTest : public testing::Test {};

TEST_F(NumericCastTest, SafePromotionTests) {
    EXPECT_EQ(numeric_cast<int>(42), 42);
    EXPECT_EQ(numeric_cast<uint32_t>(999), uint32_t(999));
    EXPECT_FLOAT_EQ(numeric_cast<float>(3.14f), 3.14f);
    EXPECT_DOUBLE_EQ(numeric_cast<double>(1.23456789), 1.23456789);
    EXPECT_EQ(numeric_cast<int32_t>(static_cast<int16_t>(42)), 42);
    EXPECT_EQ(numeric_cast<uint64_t>(static_cast<uint8_t>(255)), 255);

    EXPECT_EQ(numeric_cast<float>(true), 1.0f);
    EXPECT_EQ(numeric_cast<int>(false), 0);

    EXPECT_DOUBLE_EQ(numeric_cast<double>(3.14f), 3.14f);
}

TEST_F(NumericCastTest, SafeConversion) {
    // Widening
    EXPECT_EQ(numeric_cast<long long>(100), 100LL);
    // Narrowing within range
    EXPECT_EQ(numeric_cast<int16_t>(32767), 32767);
    EXPECT_EQ(numeric_cast<int>(100.0), 100);
}

TEST_F(NumericCastTest, OverflowDetection) {
    // Integer to smaller integer
    long long tooBigForInt = 2147483647LL + 10LL;
    EXPECT_THROW(numeric_cast<int32_t>(tooBigForInt), std::overflow_error);

    // Double to float
    double doubleTooBig = 1e40; // Max float is ~3.4e38
    EXPECT_THROW(numeric_cast<float>(doubleTooBig), std::overflow_error);
}

TEST_F(NumericCastTest, UnderflowDetection) {

    // Large negative float to integer
    double floatingTooSmall = -1e20;
    EXPECT_THROW(numeric_cast<int>(floatingTooSmall), std::underflow_error);
}

TEST_F(NumericCastTest, IntegerBounds) {
    EXPECT_THROW(numeric_cast<int8_t>(128), std::overflow_error);
    EXPECT_THROW(numeric_cast<int8_t>(-129), std::underflow_error);

    EXPECT_EQ(numeric_cast<int32_t>(2147483647u), 2147483647);
    EXPECT_THROW(numeric_cast<int32_t>(2147483648u), std::overflow_error);

    EXPECT_THROW(numeric_cast<uint32_t>(-1), std::underflow_error);
}

TEST_F(NumericCastTest, SpecialTypesChar) {

    EXPECT_EQ(numeric_cast<int32_t>(char(100)), 100);
    EXPECT_EQ(numeric_cast<uint8_t>(char(100)), 100);

    EXPECT_EQ(numeric_cast<char>(true), 1);
    EXPECT_EQ(numeric_cast<bool>(char(1)), true);

    EXPECT_THROW(numeric_cast<uint8_t>(char(-100)), std::underflow_error);

    EXPECT_FLOAT_EQ(numeric_cast<float>(char(50)), 50.f);
    EXPECT_FLOAT_EQ(numeric_cast<char>(50.f), 50.f);

    EXPECT_THROW(numeric_cast<char>(1000), std::overflow_error);
}

TEST_F(NumericCastTest, FloatToIntegerBounds) {

    EXPECT_EQ(numeric_cast<uint32_t>(100.00f), 100u);
    EXPECT_EQ(numeric_cast<uint32_t>(0.0f), 0u);
    EXPECT_EQ(numeric_cast<uint32_t>(-0.0f), 0u);
    EXPECT_THROW(numeric_cast<uint32_t>(std::numeric_limits<float>::max()), std::overflow_error);

    EXPECT_THROW(numeric_cast<uint32_t>(-0.000000000000001f), std::underflow_error);

    float too_big = 4294967296.0f;
    EXPECT_THROW(numeric_cast<uint32_t>(too_big), std::overflow_error);

    auto i32max = std::numeric_limits<int32_t>::max();
    EXPECT_THROW(numeric_cast<int32_t>(float(i32max)), std::overflow_error);

    float too_big2 = 5e10f;
    EXPECT_THROW(numeric_cast<int32_t>(too_big2), std::overflow_error);
}

TEST_F(NumericCastTest, NaNAndInfinityInhibitor) {
    double nan_v = std::numeric_limits<double>::quiet_NaN();
    double inf_v = std::numeric_limits<double>::infinity();
    EXPECT_THROW(numeric_cast<int>(nan_v), std::range_error);
    EXPECT_THROW(numeric_cast<int>(inf_v), std::overflow_error);
}

TEST_F(NumericCastTest, PrecisionLoss) {
    EXPECT_THROW(numeric_cast<int32_t>(0.9f), std::range_error);
    EXPECT_THROW(numeric_cast<int32_t>(-0.9f), std::range_error);

    EXPECT_THROW(numeric_cast<int>(3.00001f), std::range_error);
    int32_t lossy_int = 16777217;
    EXPECT_THROW(numeric_cast<float>(lossy_int), std::range_error);
    int32_t safe_int = 16777216;
    EXPECT_NO_THROW(numeric_cast<float>(safe_int));
    int64_t lossy_long = 9007199254740993LL;
    EXPECT_THROW(numeric_cast<double>(lossy_long), std::range_error);
}

TEST(RoundingCastTest, RoundCastBoundaries) {
    EXPECT_EQ(round_cast<int>(1.4f), 1);
    EXPECT_EQ(round_cast<int>(1.5f), 2);
    EXPECT_EQ(round_cast<int>(-1.5f), -2);

    auto near_max = static_cast<float>(std::numeric_limits<int32_t>::max());
    EXPECT_THROW(round_cast<int32_t>(near_max + 1000.0f), std::overflow_error);
}

TEST(RoundingCastTest, FloorCastBoundaries) {
    EXPECT_EQ(floor_cast<int>(1.9f), 1);
    EXPECT_EQ(floor_cast<int>(-1.1f), -2);
    EXPECT_EQ(floor_cast<int>(-1.0f), -1);

    double way_low = -1e20;
    EXPECT_THROW(floor_cast<int>(way_low), std::underflow_error);
}

TEST(RoundingCastTest, CeilCastBoundaries) {
    EXPECT_EQ(ceil_cast<int>(1.1f), 2);
    EXPECT_EQ(ceil_cast<int>(-1.9f), -1);
    EXPECT_EQ(ceil_cast<int>(1.0f), 1);

    double way_high = 1e20;
    EXPECT_THROW(ceil_cast<int>(way_high), std::overflow_error);
}

TEST(RoundingCastTest, SpecialValues) {
    double nan_v = std::numeric_limits<double>::quiet_NaN();
    EXPECT_THROW(round_cast<int>(nan_v), std::range_error);
    EXPECT_THROW(floor_cast<int>(nan_v), std::range_error);
    EXPECT_THROW(ceil_cast<int>(nan_v), std::range_error);
}
