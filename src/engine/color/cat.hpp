/*
 * Copyright (C) 2023  Christopher J. Howard
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

#ifndef ANTKEEPER_COLOR_CAT_HPP
#define ANTKEEPER_COLOR_CAT_HPP

#include <engine/math/matrix.hpp>
#include <engine/math/vector.hpp>

namespace color {

/// Chromatic adaption transforms (CAT).
namespace cat {

/**
 * Bradford cone response matrix.
 *
 * @see Specification ICC.1:2010 (Profile version 4.3.0.0). Image technology colour management — Architecture, profile format, and data structure, Annex E.3, pp. 102.
 * @see http://www.brucelindbloom.com/index.html?Eqn_ChromAdapt.html
 */
template <class T>
constexpr math::mat3<T> bradford =
{
	T{ 0.8951}, T{-0.7502}, T{ 0.0389},
	T{ 0.2664}, T{ 1.7135}, T{-0.0685},
	T{-0.1614}, T{ 0.0367}, T{ 1.0296}
};

/**
 * von Kries cone response matrix.
 *
 * @see http://www.brucelindbloom.com/index.html?Eqn_ChromAdapt.html
 */
template <class T>
constexpr math::mat3<T> von_kries =
{
	T{ 0.40024}, T{-0.22630}, T{0.00000},
	T{ 0.70760}, T{ 1.16532}, T{0.00000},
	T{-0.08081}, T{ 0.04570}, T{0.91822}
};

/**
 * XYZ scaling cone response matrix.
 *
 * @see http://www.brucelindbloom.com/index.html?Eqn_ChromAdapt.html
 */
template <class T>
constexpr math::mat3<T> xyz_scaling =
{
	T{1}, T{0}, T{0},
	T{0}, T{1}, T{0},
	T{0}, T{0}, T{1}
};

/**
 * Constructs a chromatic adaptation transform (CAT) matrix.
 *
 * @param w0 CIE xy chromaticity coordinates of the source illuminant.
 * @param w1 CIE xy chromaticity coordinates of the destination illuminant.
 * @param cone_response Cone response matrix.
 *
 * @return CAT matrix.
 *
 * @see Specification ICC.1:2010 (Profile version 4.3.0.0). Image technology colour management — Architecture, profile format, and data structure, Annex E.3, pp. 102.
 * @see http://www.brucelindbloom.com/index.html?Eqn_ChromAdapt.html
 */
template <class T>
[[nodiscard]] constexpr math::mat3<T> matrix(const math::vec2<T>& w0, const math::vec2<T>& w1, const math::mat3<T>& cone_response = bradford<T>) noexcept
{
	// Convert CIE xy chromaticity coordinates to CIE XYZ colors
	const math::vec3<T> w0_xyz = {w0[0] / w0[1], T{1}, (T{1} - w0[0] - w0[1]) / w0[1]};
	const math::vec3<T> w1_xyz = {w1[0] / w1[1], T{1}, (T{1} - w1[0] - w1[1]) / w1[1]};
	
	// Calculate cone response of CIE XYZ colors
	const math::vec3<T> w0_cone_response = cone_response * w0_xyz;
	const math::vec3<T> w1_cone_response = cone_response * w1_xyz;
	
	const math::mat3<T> scale =
	{
		w1_cone_response[0] / w0_cone_response[0], T{0}, T{0},
		T{0}, w1_cone_response[1] / w0_cone_response[1], T{0},
		T{0}, T{0}, w1_cone_response[2] / w0_cone_response[2],
	};
	
	return math::inverse(cone_response) * scale * cone_response;
}

} // namespace cat
} // namespace color

#endif // ANTKEEPER_COLOR_CAT_HPP
