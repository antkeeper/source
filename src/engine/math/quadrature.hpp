// SPDX-FileCopyrightText: 2023 C. J. Howard
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef ANTKEEPER_MATH_QUADRATURE_HPP
#define ANTKEEPER_MATH_QUADRATURE_HPP

#include <iterator>
#include <type_traits>

// export module math.quadrature;
// import <iterator>;
// import <type_traits>;

namespace math {

/**
 * Approximates the definite integral of a function using Simpson's 1/3 rule.
 *
 * @param f Unary function object to integrate.
 * @param first,last Range of sample points on `[first, last)`.
 *
 * @return Approximated integral of @p f.
 *
 * @see https://en.wikipedia.org/wiki/Simpson%27s_rule
 */
template<class UnaryOp, class InputIt>
[[nodiscard]] typename std::invoke_result<UnaryOp, typename std::iterator_traits<InputIt>::value_type>::type
	simpson(UnaryOp f, InputIt first, InputIt last)
{
	typedef typename std::iterator_traits<InputIt>::value_type input_type;
	typedef typename std::invoke_result<UnaryOp, input_type>::type output_type;
	typedef decltype(*last - *first) difference_type;
	
	if (first == last)
	{
		return {};
	}
	
	output_type f_a = f(*first);
	
	InputIt second = first;
	++second;
	
	if (second == last)
	{
		return f_a;
	}
	
	difference_type h = *second - *first;
	output_type f_b = f(*first + h / difference_type(2));
	output_type f_c = f(*second);
	
	output_type sum = (f_a + f_b * difference_type(4) + f_c) * h;
	
	for (first = second++; second != last; first = second++)
	{
		h = *second - *first;
		
		f_a = f_c;
		f_c = f(*second);
		f_b = f(*first + h / difference_type(2));
		
		sum += (f_a + f_b * difference_type(4) + f_c) * h;
	}
	
	return sum / difference_type(6);
}

/**
 * Approximates the definite integral of a function using the trapezoidal rule.
 *
 * @param f Unary function object to integrate.
 * @param first,last Range of sample points on `[first, last)`.
 *
 * @return Approximated integral of @p f.
 *
 * @see https://en.wikipedia.org/wiki/Trapezoidal_rule
 */
template<class UnaryOp, class InputIt>
[[nodiscard]] typename std::invoke_result<UnaryOp, typename std::iterator_traits<InputIt>::value_type>::type
	trapezoid(UnaryOp f, InputIt first, InputIt last)
{
	typedef typename std::iterator_traits<InputIt>::value_type input_type;
	typedef typename std::invoke_result<UnaryOp, input_type>::type output_type;
	typedef decltype(*last - *first) difference_type;
	
	if (first == last)
	{
		return {};
	}
	
	output_type f_a = f(*first);
	
	InputIt second = first;
	++second;
	
	if (second == last)
	{
		return f_a;
	}
	
	output_type f_b = f(*second);
	output_type sum = (f_a + f_b) * (*second - *first);
	
	for (first = second++; second != last; first = second++)
	{
		f_a = f_b;
		f_b = f(*second);
		sum += (f_a + f_b) * (*second - *first);
	}
	
	return sum / difference_type(2);
}

} // namespace math

#endif // ANTKEEPER_MATH_QUADRATURE_HPP
