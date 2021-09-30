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

#ifndef ANTKEEPER_ENTITY_COMPONENT_CONSTRAINT_SPRING_TO_HPP
#define ANTKEEPER_ENTITY_COMPONENT_CONSTRAINT_SPRING_TO_HPP

#include "entity/id.hpp"
#include "animation/spring.hpp"
#include "utility/fundamental-types.hpp"

namespace entity {
namespace component {
namespace constraint {

/**
 * Springs to a target entity.
 */
struct spring_to
{
	/// Target entity ID.
	entity::id target;
	
	/// Translation spring.
	numeric_spring<float3, float> translation;
	
	/// Rotation spring.
	numeric_spring<float4, float> rotation;
	
	/// Spring translation.
	bool spring_translation;
	
	/// Spring rotation.
	bool spring_rotation;
};

} // namespace constraint
} // namespace component
} // namespace entity

#endif // ANTKEEPER_ENTITY_COMPONENT_CONSTRAINT_SPRING_TO_HPP