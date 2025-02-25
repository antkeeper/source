// SPDX-FileCopyrightText: 2025 C. J. Howard
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef ANTKEEPER_PHYSICS_PLANCK_HPP
#define ANTKEEPER_PHYSICS_PLANCK_HPP

#include <engine/physics/constants.hpp>
#include <cmath>

namespace physics {

/// Various forms of Planck's law.
namespace planck {

/// Wavelength variant of Planck's law.
/// @param t Temperature of the blackbody, in kelvin.
/// @param lambda Wavelength of light, in meters.
/// @param c Speed of light in medium.
/// @return Spectral radiance, in watt per steradian per square meter per meter.
template <class T>
T wavelength(T t, T lambda, T c = constants::speed_of_light<T>)
{
	const T hc = constants::planck<T> * c;
	const T lambda2 = lambda * lambda;
	
	// First radiation constant (c1L)
	const T c1 = T(2) * hc * c;
	
	// Second radiation constant (c2)
	const T c2 = hc / constants::boltzmann<T>;
	
	return (c1 / (lambda2 * lambda2 * lambda)) / std::expm1(c2 / (lambda * t));
}

} // namespace planck

} // namespace physics

#endif // ANTKEEPER_PHYSICS_PLANCK_HPP
