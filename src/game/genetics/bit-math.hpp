/*
 * Copyright (C) 2020  Christopher J. Howard
 *
 * This file is part of Antkeeper source code.
 *
 * Antkeeper source code is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Antkeeper source code is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Antkeeper source code.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef ANTKEEPER_BIT_MATH_HPP
#define ANTKEEPER_BIT_MATH_HPP

namespace dna
{

/**
 * Reads bits from the least significant bits of a value and returns them in the positions marked by a mask.
 *
 * @param x Value from which bits should be read.
 * @param mask Bit mask indicating where bits should be deposited.
 * @return Bits from the least significant bits of @p x in the positions marked by @p mask.
 */
template <class T>
constexpr T bit_deposit(T x, T mask) noexcept;

/**
 * Reads bits from a value in the positions marked by a mask and returns them in the least significant bits.
 *
 * @param x Value from which bits should be read.
 * @param mask Bit mask indicating which bits to extract.
 * @return Bits of @p x from the positions marked by @p mask in the least significant bits.
 */
template <class T>
constexpr T bit_extract(T x, T mask) noexcept;

/**
 * Returns the number of set bits in a value, known as a *population count* or *Hamming weight*.
 *
 * @param x Value to count.
 * @return Number of set bits in @p x.
 */
template <typename T>
constexpr int popcount(T x) noexcept;

/**
 * Returns the number of differing bits between two values, known as *Hamming distance*.
 *
 * @param x First value.
 * @param y Second value.
 * @return Hamming distance between @px and @p y.
 */	
template <typename T>
constexpr int hamming_distance(T x, T y) noexcept;


template <typename T>
constexpr T bit_merge(T a, T b, T mask) noexcept
{
	return a ^ ((a ^ b) & mask);
}

template <class T>
constexpr T bit_pad(T x) noexcept
{
	x &= (1 << (sizeof(T) << 2)) - 1;
	
	if constexpr(sizeof(T) >= 8)
		x = (x ^ (x << 16)) & T(0x0000ffff0000ffff);
	if constexpr(sizeof(T) >= 4)
		x = (x ^ (x <<  8)) & T(0x00ff00ff00ff00ff);
	if constexpr(sizeof(T) >= 2)
		x = (x ^ (x <<  4)) & T(0x0f0f0f0f0f0f0f0f);
	
	x = (x ^ (x <<  2)) & T(0x3333333333333333);
	x = (x ^ (x <<  1)) & T(0x5555555555555555);
	
	return x;
}

template <typename T>
constexpr T bit_interleave(T a, T b) noexcept
{
	return (bit_pad(b) << 1) | bit_pad(a);
}

template <typename T>
constexpr T bit_swap_adjacent(T x) noexcept
{
	return ((x & T(0xaaaaaaaaaaaaaaaa)) >> 1) | ((x & T(0x5555555555555555)) << 1);
}

template <typename T>
constexpr T bit_shuffle_adjacent(T x, T mask) noexcept
{
	T y = bit_swap_adjacent(x);
	return bit_merge(x, y, bit_interleave<T>(mask, mask));
}

/**
 * Shuffles the adjacent bits of a value.
 *
 * @param x Value to shuffle.
 * @param g Uniform random bit generator.
 * @return Value with adjacent bits shuffled.
 */
/*
template <class T, class URBG>
constexpr T bit_shuffle_adjacent(T x, URBG&& g) noexcept
{
	return bit_swap_adjacent(x, static_cast<T>(g()));
}*/

/**
 *
 */
template <class T, class U>
U bit_splice(T a, T b, U mask)
{
	return bit_deposit(static_cast<U>(a), ~mask) | bit_deposit(static_cast<U>(b), mask);
}

/**
 * For an n-bit number with r set bits, there are `n! / ((n - r)! * r!)` permutations.
 */
template <class T>
T next_bit_permutation(T x)
{
	T y = (x | (x - 1)) + 1;
	return y | ((((y & -y) / (x & -x)) >> 1) - 1);  
}

template <class T>
constexpr T bit_deposit(T x, T mask) noexcept
{
	T result = 0;
	
	for (T i = 1; mask; i <<= 1)
	{
		if (x & i)
			result |= mask & -mask;
		mask &= mask - 1;
	}
	
	return result;
}

template <class T>
constexpr T bit_extract(T x, T mask) noexcept
{
	T result = 0;
	
	for (T i = 1; mask; i <<= 1)
	{
		if (x & mask & -mask)
			result |= i;
		mask &= mask - 1;
	}
	
	return result;
}

template <typename T>
constexpr int popcount(T x) noexcept
{
	int n = 0;
	for (; x; ++n)
		x &= x - 1;
	return n;
}

template <typename T>
inline constexpr int hamming_distance(T x, T y) noexcept
{
	return popcount(x ^ y);
}

/**
 *
 * @param mask Least significant bits indicate whether to inherit the first (0) or second (1) allele from @p a. Most significant bits indicate whether to inherit the first (0) or second (1) allele from @p b.
 */
template <class T>
constexpr T inherit(T a, T b, T mask) noexcept
{
	a = bit_shuffle_adjacent<T>(a, mask);
	b = bit_shuffle_adjacent<T>(b, mask >> (sizeof(T) << 2));
	return bit_merge<T>(a, b, T(0x5555555555555555));
}

} // namespace dna

#endif // ANTKEEPER_BIT_MATH_HPP
