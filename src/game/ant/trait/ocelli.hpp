/*
 * Copyright (C) 2021  Christopher J. Howard
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

#ifndef ANTKEEPER_GAME_ANT_TRAIT_OCELLI_HPP
#define ANTKEEPER_GAME_ANT_TRAIT_OCELLI_HPP

#include "render/model.hpp"

namespace game {
namespace ant {
namespace trait {

/**
 * Trait that describes the ocelli of an ant.
 */
struct ocelli
{
	/// Lateral ocelli present.
	bool lateral_ocelli;
	
	/// Median ocellus present.
	bool median_ocellus;
	
	/// Ocellus width, in mesosomal lengths.
	float width;
	
	/// Ocellus height, in mesosomal lengths.
	float height;
	
	/// 3D model of the lateral ocelli.
	render::model* lateral_ocelli_model;
	
	/// 3D model of the median ocellus.
	render::model* median_ocellus_model;
};

} // namespace trait
} // namespace ant
} // namespace game

#endif // ANTKEEPER_GAME_ANT_TRAIT_OCELLI_HPP