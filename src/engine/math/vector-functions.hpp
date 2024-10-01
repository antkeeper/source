// SPDX-FileCopyrightText: 2024 C. J. Howard
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef ANTKEEPER_MATH_VECTOR_FUNCTIONS_HPP
#define ANTKEEPER_MATH_VECTOR_FUNCTIONS_HPP

#include <engine/math/vector-types.hpp>
#include <engine/math/functions.hpp>
#include <algorithm>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <concepts>
#include <limits>
#include <utility>

// export module math.vector:functions;
// import :type;
// import <algorithm>;
// import <cstddef>;
// import <cstring>;
// import <cmath>;
// import <concepts>;
// import <limits>;
// import <utility>;

/// @name Vector operators
/// @{

/// Adds two vectors.
/// @param lhs Vector on the left-hand side.
/// @param rhs Vector on the right-hand side.
/// @return @p lhs + @p rhs.
/// @relates math::types::vector
template <class T, std::size_t N>
[[nodiscard]] inline constexpr math::vec<T, N> operator+(const math::vec<T, N>& lhs, const math::vec<T, N>& rhs) noexcept
{
	math::vec<T, N> result;
	for (std::size_t i = 0; i < N; ++i)
	{
		result[i] = lhs[i] + rhs[i];
	}
	return result;
}

/// Adds a vector and a scalar.
/// @param lhs Vector on the left-hand side.
/// @param rhs Scalar on the right-hand side.
/// @return @p lhs + @p rhs.
/// @relates math::types::vector
template <class T, std::size_t N>
[[nodiscard]] inline constexpr math::vec<T, N> operator+(const math::vec<T, N>& lhs, const T& rhs) noexcept
{
	math::vec<T, N> result;
	for (std::size_t i = 0; i < N; ++i)
	{
		result[i] = lhs[i] + rhs;
	}
	return result;
}

/// Adds a scalar and a vector.
/// @param lhs Scalar on the left-hand side.
/// @param rhs Vector on the right-hand side.
/// @return @p lhs + @p rhs.
/// @relates math::types::vector
template <class T, std::size_t N>
[[nodiscard]] inline constexpr math::vec<T, N> operator+(const T& lhs, const math::vec<T, N>& rhs) noexcept
{
	return rhs + lhs;
}

/// Negates a vector.
/// @param v Vector to negate.
/// @return `-v`.
/// @relates math::types::vector
template <class T, std::size_t N>
[[nodiscard]] inline constexpr math::vec<T, N> operator-(const math::vec<T, N>& v) noexcept
{
	math::vec<T, N> result;
	for (std::size_t i = 0; i < N; ++i)
	{
		result[i] = -v[i];
	}
	return result;
}

/// Subtracts two vectors.
/// @param lhs Vector on the left-hand side.
/// @param rhs Vector on the right-hand side.
/// @return @p lhs - @p rhs.
/// @relates math::types::vector
template <class T, std::size_t N>
[[nodiscard]] inline constexpr math::vec<T, N> operator-(const math::vec<T, N>& lhs, const math::vec<T, N>& rhs) noexcept
{
	math::vec<T, N> result;
	for (std::size_t i = 0; i < N; ++i)
	{
		result[i] = lhs[i] - rhs[i];
	}
	return result;
}

/// Subtracts a scalar from a vector.
/// @param lhs Vector on the left-hand side.
/// @param rhs Scalar on the right-hand side.
/// @return @p lhs - @p rhs.
/// @relates math::types::vector
template <class T, std::size_t N>
[[nodiscard]] inline constexpr math::vec<T, N> operator-(const math::vec<T, N>& lhs, const T& rhs) noexcept
{
	math::vec<T, N> result;
	for (std::size_t i = 0; i < N; ++i)
	{
		result[i] = lhs[i] - rhs;
	}
	return result;
}

/// Subtracts a vector from a scalar.
/// @param lhs Scalar on the left-hand side.
/// @param rhs Vector on the right-hand side.
/// @return @p lhs - @p rhs.
/// @relates math::types::vector
template <class T, std::size_t N>
[[nodiscard]] inline constexpr math::vec<T, N> operator-(const T& lhs, const math::vec<T, N>& rhs) noexcept
{
	math::vec<T, N> result;
	for (std::size_t i = 0; i < N; ++i)
	{
		result[i] = lhs - rhs[i];
	}
	return result;
}

/// Multiplies the elements of two vectors.
/// @param lhs Vector on the left-hand side.
/// @param rhs Vector on the right-hand side.
/// @return @p lhs * @p rhs.
/// @relates math::types::vector
template <class T, std::size_t N>
[[nodiscard]] inline constexpr math::vec<T, N> operator*(const math::vec<T, N>& lhs, const math::vec<T, N>& rhs) noexcept
{
	math::vec<T, N> result;
	for (std::size_t i = 0; i < N; ++i)
	{
		result[i] = lhs[i] * rhs[i];
	}
	return result;
}

/// Multiplies a vector by a scalar.
/// @param lhs Vector on the left-hand side.
/// @param rhs Scalar on the right-hand side.
/// @return @p lhs * @p rhs.
/// @relates math::types::vector
template <class T, std::size_t N>
[[nodiscard]] inline constexpr math::vec<T, N> operator*(const math::vec<T, N>& lhs, const T& rhs) noexcept
{
	math::vec<T, N> result;
	for (std::size_t i = 0; i < N; ++i)
	{
		result[i] = lhs[i] * rhs;
	}
	return result;
}

/// Multiplies a scalar by a vector.
/// @param lhs Scalar on the left-hand side.
/// @param rhs Vector on the right-hand side.
/// @return @p lhs * @p rhs.
/// @relates math::types::vector
template <class T, std::size_t N>
[[nodiscard]] inline constexpr math::vec<T, N> operator*(const T& lhs, const math::vec<T, N>& rhs) noexcept
{
	return rhs * lhs;
}

/// Divides the elements of a vector by the elements of another vector.
/// @param lhs Vector on the left-hand side.
/// @param rhs Vector on the right-hand side.
/// @return @p lhs / @p rhs.
/// @relates math::types::vector
template <class T, std::size_t N>
[[nodiscard]] inline constexpr math::vec<T, N> operator/(const math::vec<T, N>& lhs, const math::vec<T, N>& rhs) noexcept
{
	math::vec<T, N> result;
	for (std::size_t i = 0; i < N; ++i)
	{
		result[i] = lhs[i] / rhs[i];
	}
	return result;
}

/// Divides the elements of a vector by a scalar.
/// @param lhs Vector on the left-hand side.
/// @param rhs Scalar on the right-hand side.
/// @return @p lhs / @p rhs.
/// @relates math::types::vector
template <class T, std::size_t N>
[[nodiscard]] inline constexpr math::vec<T, N> operator/(const math::vec<T, N>& lhs, const T& rhs) noexcept
{
	math::vec<T, N> result;
	for (std::size_t i = 0; i < N; ++i)
	{
		result[i] = lhs[i] / rhs;
	}
	return result;
}

/// Divides a scalar by the elements of a vector.
/// @param lhs Scalar on the left-hand side.
/// @param rhs Vector on the right-hand side.
/// @return @p lhs / @p rhs.
/// @relates math::types::vector
template <class T, std::size_t N>
[[nodiscard]] inline constexpr math::vec<T, N> operator/(const T& lhs, const math::vec<T, N>& rhs) noexcept
{
	math::vec<T, N> result;
	for (std::size_t i = 0; i < N; ++i)
	{
		result[i] = lhs / rhs[i];
	}
	return result;
}

/// Adds two vectors, storing the result in the first vector.
/// @param lhs Vector on the left-hand side.
/// @param rhs Vector on the right-hand side.
/// @return @p lhs + @p rhs.
/// @relates math::types::vector
template <class T, std::size_t N>
inline constexpr math::vec<T, N>& operator+=(math::vec<T, N>& lhs, const math::vec<T, N>& rhs) noexcept
{
	for (std::size_t i = 0; i < N; ++i)
	{
		lhs[i] += rhs[i];
	}
	return lhs;
}

/// Adds a vector and a scalar, storing the result in the vector.
/// @param lhs Scalar on the left-hand side.
/// @param rhs Vector on the right-hand side.
/// @return @p lhs + @p rhs.
/// @relates math::types::vector
template <class T, std::size_t N>
inline constexpr math::vec<T, N>& operator+=(math::vec<T, N>& lhs, const T& rhs) noexcept
{
	for (std::size_t i = 0; i < N; ++i)
	{
		lhs[i] += rhs;
	}
	return lhs;
}

/// Subtracts two vectors, storing the result in the first vector.
/// @param lhs Vector on the left-hand side.
/// @param rhs Vector on the right-hand side.
/// @return @p lhs - @p rhs.
/// @relates math::types::vector
template <class T, std::size_t N>
inline constexpr math::vec<T, N>& operator-=(math::vec<T, N>& lhs, const math::vec<T, N>& rhs) noexcept
{
	for (std::size_t i = 0; i < N; ++i)
	{
		lhs[i] -= rhs[i];
	}
	return lhs;
}

/// Subtracts a scalar from a vector, storing the result in the vector.
/// @param lhs Vector on the left-hand side.
/// @param rhs Scalar on the right-hand side.
/// @return @p lhs - @p rhs.
/// @relates math::types::vector
template <class T, std::size_t N>
inline constexpr math::vec<T, N>& operator-=(math::vec<T, N>& lhs, const T& rhs) noexcept
{
	for (std::size_t i = 0; i < N; ++i)
	{
		lhs[i] -= rhs;
	}
	return lhs;
}

/// Multiplies two vectors, storing the result in the first vector.
/// @param lhs Vector on the left-hand side.
/// @param rhs Vector on the right-hand side.
/// @return @p lhs * @p rhs.
/// @relates math::types::vector
template <class T, std::size_t N>
inline constexpr math::vec<T, N>& operator*=(math::vec<T, N>& lhs, const math::vec<T, N>& rhs) noexcept
{
	for (std::size_t i = 0; i < N; ++i)
	{
		lhs[i] *= rhs[i];
	}
	return lhs;
}

/// Multiplies a vector by a scalar, storing the result in the vector.
/// @param lhs Vector on the left-hand side.
/// @param rhs Scalar on the right-hand side.
/// @return @p lhs * @p rhs.
/// @relates math::types::vector
template <class T, std::size_t N>
inline constexpr math::vec<T, N>& operator*=(math::vec<T, N>& lhs, const T& rhs) noexcept
{
	for (std::size_t i = 0; i < N; ++i)
	{
		lhs[i] *= rhs;
	}
	return lhs;
}

/// Divides two vectors, storing the result in the first vector.
/// @param lhs Vector on the left-hand side.
/// @param rhs Vector on the right-hand side.
/// @return @p lhs / @p rhs.
/// @relates math::types::vector
template <class T, std::size_t N>
inline constexpr math::vec<T, N>& operator/=(math::vec<T, N>& lhs, const math::vec<T, N>& rhs) noexcept
{
	for (std::size_t i = 0; i < N; ++i)
	{
		lhs[i] /= rhs[i];
	}
	return lhs;
}

/// Divides a vector by a scalar, storing the result in the vector.
/// @param lhs Vector on the left-hand side.
/// @param rhs Scalar on the right-hand side.
/// @return @p lhs / @p rhs.
/// @relates math::types::vector
template <class T, std::size_t N>
inline constexpr math::vec<T, N>& operator/=(math::vec<T, N>& lhs, const T& rhs) noexcept
{
	for (std::size_t i = 0; i < N; ++i)
	{
		lhs[i] /= rhs;
	}
	return lhs;
}

/// Logically inverts a Boolean vector.
/// @param v Vector to invert.
/// @return `!v`.
/// @relates math::types::vector
template <std::size_t N>
[[nodiscard]] inline constexpr math::bvec<N> operator!(const math::bvec<N>& v) noexcept
{
	math::bvec<N> result;
	for (std::size_t i = 0; i < N; ++i)
	{
		result[i] = !v[i];
	}
	return result;
}

/// @}

namespace math {
namespace functions {

/// @name Vector relational functions
/// @{

/// Performs an element-wise less-than comparison of two vectors.
/// @param lhs First vector.
/// @param rhs Second vector.
/// @return Boolean vector containing the result of the element comparisons.
template <class T, std::size_t N>
[[nodiscard]] inline constexpr bvec<N> less_than(const vec<T, N>& lhs, const vec<T, N>& rhs) noexcept
{
	bvec<N> result;
	for (std::size_t i = 0; i < N; ++i)
	{
		result[i] = lhs[i] < rhs[i];
	}
	return result;
}

/// Performs an element-wise less-than or equal-to comparison of two vectors.
/// @param lhs First vector.
/// @param rhs Second vector.
/// @return Boolean vector containing the result of the element comparisons.
template <class T, std::size_t N>
[[nodiscard]] inline constexpr bvec<N> less_than_equal(const vec<T, N>& lhs, const vec<T, N>& rhs) noexcept
{
	bvec<N> result;
	for (std::size_t i = 0; i < N; ++i)
	{
		result[i] = lhs[i] <= rhs[i];
	}
	return result;
}

/// Performs an element-wise greater-than comparison of two vectors.
/// @param lhs First vector.
/// @param rhs Second vector.
/// @return Boolean vector containing the result of the element comparisons.
template <class T, std::size_t N>
[[nodiscard]] inline constexpr bvec<N> greater_than(const vec<T, N>& lhs, const vec<T, N>& rhs) noexcept
{
	bvec<N> result;
	for (std::size_t i = 0; i < N; ++i)
	{
		result[i] = lhs[i] > rhs[i];
	}
	return result;
}

/// Performs an element-wise greater-than or equal-to comparison of two vectors.
/// @param lhs First vector.
/// @param rhs Second vector.
/// @return Boolean vector containing the result of the element comparisons.
template <class T, std::size_t N>
[[nodiscard]] inline constexpr bvec<N> greater_than_equal(const vec<T, N>& lhs, const vec<T, N>& rhs) noexcept
{
	bvec<N> result;
	for (std::size_t i = 0; i < N; ++i)
	{
		result[i] = lhs[i] >= rhs[i];
	}
	return result;
}

/// Compares two vectors for equality
/// @param lhs First vector.
/// @param rhs Second vector.
/// @return Boolean vector containing the result of the element comparisons.
template <class T, std::size_t N>
[[nodiscard]] inline constexpr bvec<N> equal(const vec<T, N>& lhs, const vec<T, N>& rhs) noexcept
{
	bvec<N> result;
	for (std::size_t i = 0; i < N; ++i)
	{
		result[i] = lhs[i] == rhs[i];
	}
	return result;
}

/// Compares two vectors for inequality
/// @param lhs First vector.
/// @param rhs Second vector.
/// @return Boolean vector containing the result of the element comparisons.
template <class T, std::size_t N>
[[nodiscard]] inline constexpr bvec<N> not_equal(const vec<T, N>& lhs, const vec<T, N>& rhs) noexcept
{
	bvec<N> result;
	for (std::size_t i = 0; i < N; ++i)
	{
		result[i] = lhs[i] != rhs[i];
	}
	return result;
}

/// Checks if any elements of a Boolean vector are `true`.
/// @param v Vector to be tested for truth.
/// @return `true` if any elements are `true`, `false` otherwise.
template <std::size_t N>
[[nodiscard]] inline constexpr bool any_of(const bvec<N>& v) noexcept
{
	for (std::size_t i = 0; i < N; ++i)
	{
		if (v[i])
		{
			return true;
		}
	}
	
	return false;
}

/// Checks if all elements of a Boolean vector are `true`.
/// @param v Vector to be tested for truth.
/// @return `true` if all elements are `true`, `false` otherwise.
template <std::size_t N>
[[nodiscard]] inline constexpr bool all_of(const bvec<N>& v) noexcept
{
	for (std::size_t i = 0; i < N; ++i)
	{
		if (!v[i])
		{
			return false;
		}
	}
	
	return true;
}

/// Checks if all elements of a Boolean vector are `false`.
/// @param v Vector to be tested for truth.
/// @return `true` if all elements are `false`, `false` otherwise.
template <std::size_t N>
[[nodiscard]] inline constexpr bool none_of(const bvec<N>& v) noexcept
{
	for (std::size_t i = 0; i < N; ++i)
	{
		if (v[i])
		{
			return false;
		}
	}
	
	return true;
}

/// @}

/// @name Vector functions
/// @{

/// Calculates the dot product of two vectors.
/// @param lhs First vector.
/// @param rhs Second vector.
/// @return Dot product of the two vectors.
template <class T, std::size_t N>
[[nodiscard]] inline constexpr T dot(const vec<T, N>& lhs, const vec<T, N>& rhs) noexcept
{
	T result = lhs[0] * rhs[0];
	for (std::size_t i = 1; i < N; ++i)
	{
		result += lhs[i] * rhs[i];
	}
	return result;
}

/// Calculates the cross product of two vectors.
/// @param lhs First vector.
/// @param rhs Second vector.
/// @return Cross product of the two vectors.
template <class T>
[[nodiscard]] inline constexpr vec3<T> cross(const vec3<T>& lhs, const vec3<T>& rhs) noexcept
{
	return
	{
		lhs[1] * rhs[2] - lhs[2] * rhs[1],
		lhs[2] * rhs[0] - lhs[0] * rhs[2],
		lhs[0] * rhs[1] - lhs[1] * rhs[0]
	};
}

/// Calculates the triple product of three vectors.
/// @param x First vector.
/// @param y Second vector.
/// @param z Third vector.
/// @return Triple product of the three vectors (`dot(x, cross(y, z)`).
template <class T>
[[nodiscard]] inline constexpr T triple(const vec3<T>& x, const vec3<T>& y, const vec3<T>& z) noexcept
{
	return dot(x, cross(y, z));
}

/// Calculates the square length of a vector. The square length can be calculated faster than the length because a call to `std::sqrt` is saved.
/// @param v Vector of which to calculate the square length.
/// @return Square length of the vector.
template <class T, std::size_t N>
[[nodiscard]] inline constexpr T sqr_length(const vec<T, N>& v) noexcept
{
	return dot(v, v);
}

/// Calculates the length of a vector.
/// @param v Vector of which to calculate the length.
/// @return Length of the vector.
template <std::floating_point T, std::size_t N>
[[nodiscard]] inline T length(const vec<T, N>& v)
{
	return std::sqrt(sqr_length(v));
}

/// Calculates the inverse length of a vector.
/// @param v Vector of which to calculate the inverse length.
/// @return Inverse length of the vector.
template <std::floating_point T, std::size_t N>
[[nodiscard]] inline T inv_length(const vec<T, N>& v)
{
	return T{1} / length(v);
}

/// Calculates the unit vector in the same direction as the original vector.
/// @param v Vector to normalize.
/// @return Unit vector.
template <std::floating_point T, std::size_t N>
[[nodiscard]] inline vec<T, N> normalize(const vec<T, N>& v)
{
	return v * inv_length(v);
}

/// Calculates the square distance between two points. The square distance can be calculated faster than the distance because a call to `std::sqrt` is saved.
/// @param p0 First of two points.
/// @param p1 Second of two points.
/// @return Square distance between the two points.
template <class T, std::size_t N>
[[nodiscard]] inline constexpr T sqr_distance(const vec<T, N>& p0, const vec<T, N>& p1) noexcept
{
	return sqr_length(p0 - p1);
}

/// Calculates the distance between two points.
/// @param p0 First of two points.
/// @param p1 Second of two points.
/// @return Distance between the two points.
template <std::floating_point T, std::size_t N>
[[nodiscard]] inline T distance(const vec<T, N>& p0, const vec<T, N>& p1)
{
	return length(p0 - p1);
}

/// Returns the absolute values of each element.
/// @param v Input vector.
/// @return Absolute values of input vector elements.
template <class T, std::size_t N>
[[nodiscard]] inline constexpr vec<T, N> abs(const vec<T, N>& v)
{
	vec<T, N> result;
	for (std::size_t i = 0; i < N; ++i)
	{
		result[i] = std::abs(v[i]);
	}
	return result;
}

/// Calculates the angle between two direction vectors.
/// @param from First direction vector.
/// @param to Second direction vector.
/// @return Angle between the two direction vectors, in radians.
template <std::floating_point T, std::size_t N>
[[nodiscard]] inline T angle(const vec<T, N>& from, const vec<T, N>& to)
{
	return std::acos(std::min<T>(std::max<T>(dot(from, to), T{-1}), T{1}));
}

/// Performs an element-wise ceil operation.
/// @param v Input vector.
/// @return Component-wise ceil of input vector.
template <std::floating_point T, std::size_t N>
[[nodiscard]] inline constexpr vec<T, N> ceil(const vec<T, N>& v)
{
	vec<T, N> result;
	for (std::size_t i = 0; i < N; ++i)
	{
		result[i] = std::ceil(v[i]);
	}
	return result;
}

/// Clamps the values of a vector's elements.
/// @param v Vector to clamp.
/// @param min Minimum value.
/// @param max Maximum value.
/// @return Clamped vector.
template <class T, std::size_t N>
[[nodiscard]] inline constexpr vec<T, N> clamp(const vec<T, N>& v, const vec<T, N>& min, const vec<T, N>& max) noexcept
{
	vec<T, N> result;
	for (std::size_t i = 0; i < N; ++i)
	{
		result[i] = v[i] < min[i] ? min[i] : max[i] < v[i] ? max[i] : v[i];
	}
	return result;
}

/// @copydoc clamp(const vec<T, N>&, const vec<T, N>&, const vec<T, N>&)
template <class T, std::size_t N>
[[nodiscard]] inline constexpr vec<T, N> clamp(const vec<T, N>& v, const T& min, const T& max) noexcept
{
	vec<T, N> result;
	for (std::size_t i = 0; i < N; ++i)
	{
		result[i] = v[i] < min ? min : max < v[i] ? max : v[i];
	}
	return result;
}

/// Clamps the length of a vector.
/// @param v Vector to clamp.
/// @param max_length Maximum length.
/// @return Length-clamped vector.
template <std::floating_point T, std::size_t N>
[[nodiscard]] vec<T, N> clamp_length(const vec<T, N>& v, T max_length)
{
	const auto length2 = sqr_length(v);
	return (length2 > max_length * max_length) ? (v * (max_length / std::sqrt(length2))) : v;
}

/// Performs an element-wise floor operation.
/// @param v Input vector.
/// @return Component-wise floor of input vector.
template <std::floating_point T, std::size_t N>
[[nodiscard]] inline constexpr vec<T, N> floor(const vec<T, N>& v)
{
	vec<T, N> result;
	for (std::size_t i = 0; i < N; ++i)
	{
		result[i] = std::floor(v[i]);
	}
	return result;
}

/// Performs a multiply-add operation.
/// @param x Input vector
/// @param y Value to multiply.
/// @param z Value to add.
/// @return Vector containing the multiply-add results.
template <class T, std::size_t N>
[[nodiscard]] inline constexpr vec<T, N> fma(const vec<T, N>& x, const vec<T, N>& y, const vec<T, N>& z)
{
	vec<T, N> result;
	for (std::size_t i = 0; i < N; ++i)
	{
		result[i] = std::fma(x[i], y[i], z[i]);
	}
	return result;
}

/// @copydoc fma(const vec<T, N>&, const vec<T, N>&, const vec<T, N>&)
template <class T, std::size_t N>
[[nodiscard]] inline constexpr vec<T, N> fma(const vec<T, N>& x, T y, T z)
{
	vec<T, N> result;
	for (std::size_t i = 0; i < N; ++i)
	{
		result[i] = std::fma(x[i], y, z);
	}
	return result;
}

/// Returns a vector containing the fractional part of each element.
/// @param v Input vector.
/// @return Fractional parts of input vector.
template <std::floating_point T, std::size_t N>
[[nodiscard]] inline constexpr vec<T, N> fract(const vec<T, N>& v)
{
	vec<T, N> result;
	for (std::size_t i = 0; i < N; ++i)
	{
		result[i] = v[i] - std::floor(v[i]);
	}
	return result;
}

/// Returns a vector containing the minimum elements of two vectors.
/// @param x First vector.
/// @param y Second vector.
/// @return Minimum elements of the two vectors.
template <class T, std::size_t N>
[[nodiscard]] inline constexpr vec<T, N> min(const vec<T, N>& x, const vec<T, N>& y) noexcept
{
	vec<T, N> result;
	for (std::size_t i = 0; i < N; ++i)
	{
		result[i] = y[i] < x[i] ? y[i] : x[i];
	}
	return result;
}

/// Returns a vector containing the maximum elements of two vectors.
/// @param x First vector.
/// @param y Second vector.
/// @return Maximum elements of the two vectors.
template <class T, std::size_t N>
[[nodiscard]] inline constexpr vec<T, N> max(const vec<T, N>& x, const vec<T, N>& y) noexcept
{
	vec<T, N> result;
	for (std::size_t i = 0; i < N; ++i)
	{
		result[i] = x[i] < y[i] ? y[i] : x[i];
	}
	return result;
}

/// Returns a reference to the smallest element in a vector.
/// @param v Input vector.
/// @return Reference to the smallest element in @p v.
template <class T, std::size_t N>
[[nodiscard]] constexpr const T& min_element(const vec<T, N>& v) noexcept
{
	const T* result = &v[0];
	for (std::size_t i = 1; i < N; ++i)
	{
		if (*result > v[i])
		{
			result = &v[i];
		}
	}
	return *result;
}

/// Returns a reference to the greatest element in a vector.
/// @param v Input vector.
/// @return Reference to the greatest element in @p v.
template <class T, std::size_t N>
[[nodiscard]] inline constexpr const T& max_element(const vec<T, N>& v) noexcept
{
	const T* result = &v[0];
	for (std::size_t i = 1; i < N; ++i)
	{
		if (*result < v[i])
		{
			result = &v[i];
		}
	}
	return *result;
}

/// Calculates the element-wise remainder of the division operation `lhs / rhs`.
/// @param lhs Left-hand side.
/// @param rhs Right-hand side.
/// @return Remainders of `lhs / rhs`.
template <std::floating_point T, std::size_t N>
[[nodiscard]] constexpr vec<T, N> mod(const vec<T, N>& lhs, const vec<T, N>& rhs)
{
	vec<T, N> result;
	for (std::size_t i = 0; i < N; ++i)
	{
		result[i] = std::fmod(lhs[i], rhs[i]);
	}
	return result;
}

/// @copydoc mod(const vec<T, N>&, const vec<T, N>&)
template <std::floating_point T, std::size_t N>
[[nodiscard]] constexpr vec<T, N> mod(const vec<T, N>& lhs, T rhs)
{
	vec<T, N> result;
	for (std::size_t i = 0; i < N; ++i)
	{
		result[i] = std::fmod(lhs[i], rhs);
	}
	return result;
}

/// @copydoc mod(const vec<T, N>&, const vec<T, N>&)
template <std::floating_point T, std::size_t N>
[[nodiscard]] constexpr vec<T, N> mod(T lhs, const vec<T, N>& rhs)
{
	vec<T, N> result;
	for (std::size_t i = 0; i < N; ++i)
	{
		result[i] = std::fmod(lhs, rhs[i]);
	}
	return result;
}

/// Linearly interpolates between two vectors.
/// @tparam T Floating-point type.
/// @param a First vector.
/// @param b Second vector.
/// @param t Interpolation factor.
/// @return `(b - a) * t + a`
template <class T, std::size_t N>
[[nodiscard]] inline constexpr vec<T, N> lerp(const vec<T, N>& a, const vec<T, N>& b, T t) noexcept
{
	return (b - a) * t + a;
}

/// Raises each element to a power.
/// @param x Input vector.
/// @param y Exponent.
/// @return Vector with its elements raised to the given power.
template <std::floating_point T, std::size_t N>
[[nodiscard]] inline vec<T, N> pow(const vec<T, N>& x, const vec<T, N>& y)
{
	vec<T, N> result;
	for (std::size_t i = 0; i < N; ++i)
	{
		result[i] = std::pow(x[i], y[i]);
	}
	return result;
}

/// @copydoc pow(const vec<T, N>&, const vec<T, N>&)
template <std::floating_point T, std::size_t N>
[[nodiscard]] inline vec<T, N> pow(const vec<T, N>& x, T y)
{
	vec<T, N> result;
	for (std::size_t i = 0; i < N; ++i)
	{
		result[i] = std::pow(x[i], y);
	}
	return result;
}

/// Performs an element-wise round operation.
/// @param v Input vector
/// @return Component-wise round of input vector.
template <std::floating_point T, std::size_t N>
[[nodiscard]] inline constexpr vec<T, N> round(const vec<T, N>& v)
{
	vec<T, N> result;
	for (std::size_t i = 0; i < N; ++i)
	{
		result[i] = std::round(v[i]);
	}
	return result;
}

/// Returns a vector containing the signs of each element.
/// @param v Input vector
/// @return Signs of input vector elements.
template <std::floating_point T, std::size_t N>
[[nodiscard]] inline constexpr vec<T, N> sign(const vec<T, N>& v)
{
	vec<T, N> result;
	for (std::size_t i = 0; i < N; ++i)
	{
		result[i] = std::copysign(T{1}, v[i]);
	}
	return result;
}

/// Calculates the signed angle between two direction vectors about axis.
/// @param from First direction vector.
/// @param to Second direction vector.
/// @param axis Axis direction vector.
/// @return Signed angle between @p from and @p to about @p axis, in radians.
template <std::floating_point T>
[[nodiscard]] inline T signed_angle(const vec3<T>& from, const vec3<T>& to, const vec3<T>& axis)
{
	return std::atan2(triple(axis, from, to), dot(from, to));
}

/// Takes the square root of each element.
/// @param v Input vector
/// @return Square roots of the input vector elements.
template <std::floating_point T, std::size_t N>
[[nodiscard]] inline vec<T, N> sqrt(const vec<T, N>& v)
{
	vec<T, N> result;
	for (std::size_t i = 0; i < N; ++i)
	{
		result[i] = std::sqrt(v[i]);
	}
	return result;
}

/// Calculates the sum of all elements in a vector.
/// @param v Vector to sum.
/// @return Sum of the vector's elements.
template <class T, std::size_t N>
[[nodiscard]] inline constexpr T sum(const vec<T, N>& v) noexcept
{
	T result = v[0];
	for (std::size_t i = 1; i < N; ++i)
	{
		result += v[i];
	}
	return result;
}

/// Makes an *m*-dimensional vector by rearranging and/or duplicating elements of an *n*-dimensional vector.
/// @tparam Indices List of indices of elements in @p v.
/// @tparam T Vector element type.
/// @tparam N Number of dimensions in @p v.
/// @param v Vector to swizzle.
/// @return Vector containing elements from @p v in the order specified by @p Indices. The size of the returned vector is equivalent to the number of indices in @p Indices.
template <std::size_t... Indices, class T, std::size_t N>
[[nodiscard]] inline constexpr vec<T, sizeof...(Indices)> swizzle(const vec<T, N>& v) noexcept
{
	return {v[Indices]...};
}

/// Performs an element-wise trunc operation.
/// @param v Input vector
/// @return Component-wise trunc of input vector.
template <std::floating_point T, std::size_t N>
[[nodiscard]] inline constexpr vec<T, N> trunc(const vec<T, N>& v)
{
	vec<T, N> result;
	for (std::size_t i = 0; i < N; ++i)
	{
		result[i] = std::trunc(v[i]);
	}
	return result;
}

/// @}

} // namespace functions

// Bring math::functions into math namespace
using namespace functions;

} // namespace math

#endif // ANTKEEPER_MATH_VECTOR_FUNCTIONS_HPP
