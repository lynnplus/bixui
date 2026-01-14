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

/**
 * @file size.h
 * @brief 2D size (width and height) type definitions.
 */

#pragma once

#include <bixlib/export_macro.h>
#include <bixlib/utils/numeric.h>

namespace bix {
namespace geom {

/**
 * A template structure representing the dimensions of a 2D object.
 * @tparam T The numeric type for width and height which must satisfy the Real concept.
 */
template <Real T>
class BIX_PUBLIC SizeT {

public:
    T width{0}, height{0}; ///< The horizontal and vertical dimensions.

    /** Default constructor that initializes dimensions to zero. */
    constexpr SizeT() = default;

    /**
     * Constructor that initializes dimensions with explicit width and height.
     *
     * @param w The initial width.
     * @param h The initial height.
     */
    constexpr SizeT(T w, T h) : width(w), height(h) {}

    SizeT(const SizeT& other) = default;
    SizeT& operator=(const SizeT& other) = default;

    /**
     * Constructs a SizeT from another compatible numeric type.
     *
     * This constructor utilizes the Upcastable concept to allow only safe,
     * non-narrowing conversions (e.g., SizeI to SizeF). It is marked as
     * explicit to ensure that dimensions are not promoted implicitly
     * during layout calculations or UI scaling.
     *
     * @tparam From The source numeric type of the dimensions.
     * @param other The source size object to convert from.
     */
    template <typename From>
    requires Upcastable<T, From>
    constexpr explicit SizeT(const SizeT<From>& other) noexcept
        : width(static_cast<T>(other.width))
        , height(static_cast<T>(other.height)) {}

    /**
     * Assigns from another compatible size type.
     *
     * Only available if the source numeric type From can be safely
     * upcasted to T without precision loss.
     *
     * @tparam From The source numeric type.
     * @param other The source size object to assign from.
     * @return A reference to this instance.
     */
    template <typename From>
    requires Upcastable<T, From>
    constexpr SizeT& operator=(const SizeT<From>& other) noexcept {
        width = static_cast<T>(other.width);
        height = static_cast<T>(other.height);
        return *this;
    }

    /**
     * Returns a size instance with dimensions set to -1 representing an invalid state.
     * @return A size with both width and height as -1.
     */
    static constexpr SizeT invalid() { return SizeT(static_cast<T>(-1), static_cast<T>(-1)); }

    /**
     * Creates a new size from another size instance of a different numeric type.
     *
     * @note This operation performs an explicit cast; converting from floating-point to integral types will result in
     * truncation.
     * @tparam U The coordinate type of the source size.
     * @param other The source size to convert from.
     * @return A new size instance with dimensions cast to type T.
     */
    template <typename U>
    static constexpr SizeT from(const SizeT<U>& other) noexcept {
        return SizeT(static_cast<T>(other.width), static_cast<T>(other.height));
    }

    /**
     * Converts the current size instance to a specified numeric type.
     * @note Converting from floating-point to integral types will result in truncation.
     * @tparam To The target numeric type.
     * @return A new size instance of type To.
     */
    template <typename To>
    constexpr SizeT<To> as() const noexcept {
        return SizeT<To>::from(*this);
    }

    /**
     * Checks if both width and height are zero within a given epsilon threshold.
     * @param epsilon The tolerance value used for floating-point comparison.
     * @return True if dimensions are effectively zero, false otherwise.
     */
    constexpr bool isZero(T epsilon = math::default_eps_v<T>) const noexcept {
        if constexpr (std::is_floating_point_v<T>) {
            return math::fuzzyIsZero(width, epsilon) && math::fuzzyIsZero(height, epsilon);
        } else {
            BIX_UNUSED(epsilon)
            return width == 0 && height == 0;
        }
    }

    /**
     * Returns a size instance where each dimension is the smallest integer not less than the original value.
     * @return A new size instance with integer dimensions of type int.
     */
    SizeT<int> ceiled() const noexcept { return {math::ceil_cast<int>(width), math::ceil_cast<int>(height)}; }

    /**
     * Checks if the size is valid with non-negative dimensions.
     * @return True if both dimensions are greater than or equal to zero.
     */
    constexpr bool isValid() const noexcept { return width >= static_cast<T>(0) && height >= static_cast<T>(0); }

    /**
     * Checks if the size is empty where at least one dimension is zero or negative.
     * @return True if either width or height is less than or equal to zero.
     */
    bool isEmpty() const noexcept { return width <= static_cast<T>(0) || height <= static_cast<T>(0); }

    /**
     * Calculates the width-to-height ratio of the dimensions.
     * @warning If the height is zero, this method returns 0 to avoid division by zero.
     * @note The return type is T to maintain precision consistency with the class template.
     * @return The aspect ratio as type T.
     */
    constexpr T aspectRatio() const { return height != 0 ? width / height : static_cast<T>(0); }

    /**
     * Checks if this size is approximately equal to another size.
     *
     * For floating-point types, it uses the provided epsilon. For integral types,
     * it performs an exact comparison.
     *
     * @param other The size to compare with.
     * @param epsilon The tolerance threshold for floating-point comparison.
     * @return True if the sizes are considered equal.
     */
    constexpr bool equals(const SizeT& other, T epsilon = math::default_eps_v<T>) const noexcept {
        if constexpr (std::is_floating_point_v<T>) {
            return math::fuzzyEqual(width, other.width, epsilon) && math::fuzzyEqual(height, other.height, epsilon);
        } else {
            BIX_UNUSED(epsilon)
            return width == other.width && height == other.height;
        }
    }

    /**
     * @name Arithmetic Operators
     * @{
     */
    /**
     * Equality operator for strict comparison.
     *
     * @note For floating-point types, this performs a bitwise-like equality check
     * which might be affected by precision issues. Use #equals() for fuzzy comparison.
     */
    constexpr bool operator==(const SizeT& rhs) const noexcept {
        return math::exactlyEqual(width, rhs.width) && math::exactlyEqual(height, rhs.height);
    }

    /**
     * Adds two sizes and ensures the resulting dimensions are not less than zero.
     * @note If adding a size with negative dimensions results in a value below zero, it is clamped to zero.
     * @param rhs The size instance to add.
     * @return A new size instance with dimensions clamped at a minimum of zero.
     */
    SizeT operator+(const SizeT& rhs) const noexcept {
        return {std::max(width + rhs.width, static_cast<T>(0)), std::max(height + rhs.height, static_cast<T>(0))};
    }

    /**
     * Subtracts another size from the current size and ensures the resulting dimensions are not less than zero.
     * @note If the subtraction results in a value below zero, it is clamped to zero to maintain a valid size.
     * @param rhs The size instance to subtract.
     * @return A new size instance with dimensions clamped at a minimum of zero.
     */
    SizeT operator-(const SizeT& rhs) const noexcept {
        return {std::max(width - rhs.width, static_cast<T>(0)), std::max(height - rhs.height, static_cast<T>(0))};
    }

    /**
     * Adds another size to the current instance and clamps the resulting dimensions to a minimum of zero.
     * @param rhs The size instance to add.
     * @return A reference to the updated current size instance.
     */
    SizeT& operator+=(const SizeT& rhs) noexcept {
        width = std::max(width + rhs.width, static_cast<T>(0));
        height = std::max(height + rhs.height, static_cast<T>(0));
        return *this;
    }

    /**
     * Subtracts another size from the current instance and clamps the resulting dimensions to a minimum of zero.
     * @param rhs The size instance to subtract.
     * @return A reference to the updated current size instance.
     */
    SizeT& operator-=(const SizeT& rhs) noexcept {
        width = std::max(width - rhs.width, static_cast<T>(0));
        height = std::max(height - rhs.height, static_cast<T>(0));
        return *this;
    }

    constexpr SizeT operator*(T factor) const noexcept { return {width * factor, height * factor}; }

    /**
     * Scales the size dimensions by a floating-point factor and deduces the most appropriate result type.
     * @note This operator uses type promotion to find a common type that can hold the result without unnecessary
     * precision loss.
     * @tparam F The floating-point multiplier type.
     * @param factor The scaling multiplier.
     * @return A new size instance with the common numeric type between T and F.
     */
    template <std::floating_point F>
    requires(!std::same_as<T, F>)
    constexpr SizeT<std::common_type_t<T, F>> operator*(F factor) const noexcept {
        using ResultT = std::common_type_t<T, F>;
        return SizeT<ResultT>{
            static_cast<ResultT>(width) * static_cast<ResultT>(factor),
            static_cast<ResultT>(height) * static_cast<ResultT>(factor)
        };
    }

    ///@}
};

} // namespace geom

/** Type alias for an integer size. */
using SizeI = geom::SizeT<int>;
/** Type alias for a single-precision floating-point size. */
using SizeF = geom::SizeT<float>;
/** Default size type alias using SizeF. */
using Size = SizeF;
} // namespace bix
