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

#include <bit>
#include <bitset>
#include <initializer_list>
#include <type_traits>

namespace bix {
/**
 *
 * a type-safe bit flags container for enumeration types
 *
 * This template class provides a type-safe wrapper for managing bit flags
 * using enumeration types. It ensures that only valid enum values can be used
 * as flags and provides intuitive operations for flag manipulation.
 *
 * @tparam Enum The enumeration type to be used for flags.
 *  The template parameter Enum must satisfy:
 * - must be an enumeration type
 * - underlying type must be unsigned
 *
 * @note All operations are constexpr and can be evaluated at compile-time
 * @note All methods are noexcept and guarantee not to throw exceptions
 * @note Designed for enumeration values that are powers of two (1<<0, 1<<1, etc.)
 * @see std::bitset for similar functionality with dynamic size
 */
template <class Enum>
requires std::is_enum_v<Enum> && std::is_unsigned_v<std::underlying_type_t<Enum>>
class Flags {
public:
    using EnumType = Enum;

    /**
     * The underlying integral type used for flag storage
     *
     * Extracts the underlying type of the enum using std::underlying_type_t
     * to ensure type-safe bit operations.
     */
    using ValueType = std::underlying_type_t<Enum>;

    /**
     * Zero constant representing no flags set
     *
     * Provides a type-safe zero value for comparison and initialization of flag containers.
     */
    constexpr static ValueType ZERO = ValueType(0);

    /**
     * Maximum number of flags that can be stored in the flags' container.
     *
     * This constexpr static constant defines the upper limit of flags
     * based on the underlying storage type. The value is calculated as
     * the size of the #ValueType in bytes multiplied by 8 (bits per byte),
     * This represents the total bit capacity available for flag storage.
     */
    constexpr static size_t MAX_SIZE = sizeof(ValueType) * CHAR_BIT;

    /**
     * Default constructor that initializes all flags to unset state #ZERO.
     */
    constexpr Flags() noexcept { mValue = ZERO; }

    /**
     * Explicit constructor from enum value
     *
     * @param flag The enum value to initialize the flags with
     */
    constexpr explicit Flags(Enum flag) noexcept { mValue = ValueType(flag); }

    /**
     * Initializes the flags container with the specified enum values.
     *
     * This constructor initializes the flags by setting all flags provided in the initializer list.
     * It performs a bitwise OR operation for each flag in the list to set the corresponding bits.
     * @param flags The initializer list of enum values to set
     */
    constexpr Flags(std::initializer_list<Enum> flags) noexcept {
        for (auto flag : flags) {
            mValue = mValue | ValueType(flag);
        }
    }

    /**
     * Gets the underlying integer value representing all flags
     *
     * @return The integer value containing all flag states
     */
    constexpr ValueType value() const noexcept { return mValue; }

    /**
     * Converts the flags container to a std::bitset representation.
     *
     * This constexpr function creates a std::bitset object that mirrors
     * the current state of all flags in the container.
     * @tparam Size The size of the bitset, defaults to #MAX_SIZE
     * @return A std::bitset containing the current flag states
     */
    template <size_t Size = MAX_SIZE>
    constexpr std::bitset<Size> bits() const noexcept {
        return mValue;
    }

    /**
     * Checks if the specified flag is set in the flags' container.
     *
     * @param[in] flag The enumeration value to test
     * @return true if the specified flag is set, false otherwise
     * @see testFlags()
     * @note Special case: if the provided flag are #ZERO, returns true only if current flags' container are also #ZERO
     */
    constexpr bool testFlag(Enum flag) const noexcept { return testFlags(Flags(flag)); }

    /**
     * Checks if all specified flags are set in the flags' container.
     *
     * This constexpr function checks whether all flags provided in the parameter
     * are currently active in the flags' collection. It performs a bitwise comparison
     * to verify that all the specified flag bits are set simultaneously.
     *
     * @param[in] flags The combination of enumeration flags to test
     * @return true if all specified flags are set, false otherwise
     * @note This function is constexpr and can be evaluated at compile-time
     * @note Special case: if the provided flags are #ZERO, returns true only if current flags are also #ZERO
     * @see testFlag(),testAnyFlags()
     * @warning The flags parameter must be a valid combination of enumeration values
     *
     */
    constexpr bool testFlags(Flags flags) const noexcept {
        if (flags.mValue == ZERO) {
            return mValue == ZERO;
        }
        return (mValue & flags.mValue) == flags.mValue;
    }

    /**
     * Checks if any of the specified flags are set in the flags' container.
     *
     * This constexpr function tests whether at least one flag from the provided
     * flags parameter is currently active in the flags' collection. It performs
     * a bitwise comparison to determine if any of the specified flag bits are set.
     *
     * @param[in] flags The combination of enumeration flags to test
     * @return true if any of the specified flags are set, false if none are set
     */
    constexpr bool testAnyFlags(Flags flags) const noexcept { return (flags.mValue & flags.mValue) != ZERO; }

    /**
     * Checks if any flag is set in the flags' container.
     * @return true if at least one flag is set, false otherwise
     */
    constexpr bool testAny() const noexcept { return mValue != ZERO; }

    /**
     * Sets the specified flag to the enabled state.
     * @param[in] flag The enumeration value to enable
     * @return Reference to the modified flags object for method chaining
     */
    constexpr Flags& on(Enum flag) noexcept { return on(Flags(flag)); }

    constexpr Flags& on(Flags flags) noexcept {
        mValue |= flags.mValue;
        return *this;
    }

    /**
     * Disables the specified flag in the flags' container.
     *
     * This constexpr function turns off the given enumeration flag by clearing
     * the corresponding bit in the flags' collection. The function returns a
     * reference to the modified flags object, supporting method chaining.
     *
     * @param[in] flag The enumeration value to disable
     * @return Reference to the modified flags object for method chaining
     * @see on(Enum), toggle(), testFlag()
     */
    constexpr Flags& off(Enum flag) noexcept { return off(Flags(flag)); }

    constexpr Flags& off(Flags flags) noexcept {
        mValue &= ~flags.mValue;
        return *this;
    }

    constexpr Flags& setFlag(Enum flag, bool enable) noexcept { return enable ? on(Flags(flag)) : off(Flags(flag)); }

    /**
     * Toggles the state of the specified flag in the flags' container.
     *
     * This constexpr function inverts the state of the given enumeration flag by flipping
     * the corresponding bit in the flags' collection. If the flag is currently set, it will
     * be cleared; if it's cleared, it will be set. The function returns a reference to the
     * modified flags object, enabling method chaining.
     *
     * @param[in] flag The enumeration value to toggle
     * @return Reference to the modified flags object for method chaining
     */
    constexpr Flags& toggle(Enum flag) noexcept { return toggle(Flags(flag)); }

    constexpr Flags& toggle(Flags flags) noexcept {
        mValue ^= flags.mValue;
        return *this;
    }

    /**
     * Toggles the state of all flags in the flags container.
     *
     * This constexpr function inverts the state of every flag by flipping
     * all bits in the flags collection. Each flag that was set becomes cleared,
     * and each flag that was cleared becomes set.
     *
     * The function returns a reference to the modified flags object, enabling method chaining.
     *
     * @tparam Size The number of flags to toggle, defaults to #MAX_SIZE
     * @return Reference to the modified flags object for method chaining
     * @note Useful for quickly inverting the entire state of the flags container
     * @see toggle(), on(), off()
     */
    template <size_t Size = MAX_SIZE>
    constexpr Flags& toggleAll() noexcept {
        if constexpr (Size == MAX_SIZE) {
            mValue = ~mValue;
        } else {
            mValue ^= (ValueType(1) << Size) - 1;
        }
        return *this;
    }

    /**
     * Counts the number of flags set in the flags container.
     *
     * This constexpr function efficiently calculates the number of
     * active flags using population count algorithms. It provides
     * optimal performance for both compile-time and runtime evaluation.
     *
     * @return The number of flags currently set
     * @see std::popcount()
     */
    constexpr int count() const noexcept { return std::popcount(mValue); }

    /**
     * Retains only the flags that are also set in the given Flags object
     * @param flags The Flags object whose flags should be retained
     * @return Reference to the modified flags object for method chaining
     * @see #operator&=(Flags)
     */
    constexpr Flags& retainFlags(Flags flags) noexcept {
        mValue &= flags.mValue;
        return *this;
    }

    /**
     * Clears all flags by resetting the internal value to #ZERO
     */
    constexpr void clear() noexcept { mValue = ZERO; }

    constexpr Flags operator&(Flags flags) const noexcept { return Flags(mValue & flags.mValue); }

    constexpr Flags operator|(Flags flags) const noexcept { return Flags(mValue | flags.mValue); }

    constexpr Flags operator~() const noexcept { return Flags(~mValue); }

    constexpr Flags operator^(Flags flags) const noexcept { return Flags(mValue ^ flags.mValue); }

    constexpr Flags& operator&=(Flags flags) noexcept {
        mValue &= flags.mValue;
        return *this;
    }

    constexpr Flags& operator|=(Flags flags) noexcept {
        mValue |= flags.mValue;
        return *this;
    }

    constexpr Flags& operator^=(Flags flags) noexcept {
        mValue ^= flags.mValue;
        return *this;
    }

    constexpr explicit operator bool() const noexcept { return mValue != ZERO; }

    constexpr bool operator!() const noexcept { return !mValue; }

    constexpr bool operator==(const Flags& flag) const noexcept { return mValue == flag.mValue; }

private:
    ValueType mValue = ZERO;

    constexpr explicit Flags(ValueType value) noexcept { mValue = value; }
};

template <class Enum, class Enable = void>
struct IsFlagsEnumType : std::false_type {};

template <class Enum>
requires IsFlagsEnumType<Enum>::value
constexpr Flags<Enum> operator|(Enum lhs, Enum rhs) {
    return Flags(lhs) | Flags(rhs);
}

#define BIX_DECLARE_ENUM_FLAGS(Enum)                  \
    template <>                                       \
    struct IsFlagsEnumType<Enum> : std::true_type {};

} // namespace bix
