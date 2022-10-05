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

#include "game/system/behavior.hpp"
#include "game/component/behavior.hpp"
#include "entity/id.hpp"

namespace game {
namespace system {

behavior::behavior(entity::registry& registry):
	updatable(registry)
{}

void behavior::update(double t, double dt)
{
	entity::ebt::context context;
	context.registry = &registry;

	registry.view<component::behavior>().each(
		[&](entity::id entity_id, auto& behavior)
		{
			if (behavior.behavior_tree)
			{
				context.entity_id = entity_id;
				behavior.behavior_tree->execute(context);
			}
		});
}

} // namespace system
} // namespace game