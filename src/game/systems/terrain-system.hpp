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

#ifndef ANTKEEPER_TERRAIN_SYSTEM_HPP
#define ANTKEEPER_TERRAIN_SYSTEM_HPP

#include "entity-system.hpp"
#include "game/components/terrain-component.hpp"

class terrain;
class resource_manager;
class mesh;
class model;
class image;

class terrain_system: public entity_system
{
public:
	terrain_system(entt::registry& registry, ::resource_manager* resource_manager);
	~terrain_system();
	virtual void update(double t, double dt);
	
	/**
	 * Sets the size of a single terrain patch.
	 */
	void set_patch_size(float size);

private:
	mesh* generate_terrain_mesh(float size, int subdivisions);
	model* generate_terrain_model(mesh* terrain_mesh);
	void project_terrain_mesh(mesh* terrain_mesh, const ecs::terrain_component& component);
	void update_terrain_model(model* terrain_model, mesh* terrain_mesh);

	void on_terrain_construct(entt::registry& registry, entt::entity entity, ecs::terrain_component& component);
	void on_terrain_destroy(entt::registry& registry, entt::entity entity);

	resource_manager* resource_manager;
	float patch_size;
	float heightmap_size;
	float heightmap_scale;
	image* heightmap;
};

#endif // ANTKEEPER_TERRAIN_SYSTEM_HPP
