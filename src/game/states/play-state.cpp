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

#include "animation/ease.hpp"
#include "animation/screen-transition.hpp"
#include "configuration.hpp"
#include "debug/logger.hpp"
#include "ecs/archetype.hpp"
#include "ecs/components/cavity-component.hpp"
#include "ecs/components/copy-transform-component.hpp"
#include "ecs/components/copy-translation-component.hpp"
#include "ecs/components/model-component.hpp"
#include "ecs/components/snap-component.hpp"
#include "ecs/components/samara-component.hpp"
#include "ecs/components/terrain-component.hpp"
#include "ecs/components/tool-component.hpp"
#include "ecs/components/transform-component.hpp"
#include "ecs/components/camera-follow-component.hpp"
#include "ecs/components/orbit-component.hpp"
#include "ecs/commands.hpp"
#include "game/game-context.hpp"
#include "game/states/game-states.hpp"
#include "math/math.hpp"
#include "nest.hpp"
#include "renderer/material.hpp"
#include "rasterizer/texture-2d.hpp"
#include "rasterizer/texture-filter.hpp"
#include "rasterizer/texture-wrapping.hpp"
#include "renderer/model.hpp"
#include "renderer/passes/sky-pass.hpp"
#include "resources/resource-manager.hpp"
#include "scene/model-instance.hpp"
#include "scene/collection.hpp"
#include "scene/camera.hpp"
#include "scene/ambient-light.hpp"
#include "scene/directional-light.hpp"
#include "scene/directional-light.hpp"
#include "ecs/systems/control-system.hpp"
#include "ecs/systems/camera-system.hpp"
#include "ecs/systems/render-system.hpp"
#include "ecs/systems/tool-system.hpp"
#include "ecs/systems/weather-system.hpp"
#include "ecs/systems/solar-system.hpp"
#include "ecs/systems/astronomy-system.hpp"
#include "game/biome.hpp"
#include "utility/fundamental-types.hpp"
#include "utility/gamma.hpp"

#include "utility/bit-math.hpp"
#include "genetics/genetics.hpp"
#include <iostream>
#include <bitset>
#include <ctime>


void play_state_enter(game_context* ctx)
{
	debug::logger* logger = ctx->logger;
	logger->push_task("Entering play state");
	
	// Load biome
	if (ctx->option_biome.has_value())
	{
		ctx->biome = ctx->resource_manager->load<biome>(ctx->option_biome.value() + ".bio");
	}
	else
	{
		ctx->biome = ctx->resource_manager->load<biome>("grassland.bio");
	}
	
	// Apply biome parameters to scene
	sky_pass* sky_pass = ctx->overworld_sky_pass;
	sky_pass->set_enabled(true);
	sky_pass->set_sky_model(ctx->resource_manager->load<model>("sky-dome.mdl"));
	sky_pass->set_moon_model(ctx->resource_manager->load<model>("moon.mdl"));
	
	ctx->weather_system->set_universal_time(0.0);
	
	ctx->solar_system->set_universal_time(0.0);
	
	ctx->astronomy_system->set_observer_location(double3{4.26352e-5, ctx->biome->location[0], ctx->biome->location[1]});
	ctx->astronomy_system->set_universal_time(0.0);
	ctx->astronomy_system->set_obliquity(math::radians(23.4393));
	ctx->astronomy_system->set_axial_rotation_at_epoch(math::radians(280.4606));
	ctx->astronomy_system->set_axial_rotation_speed(math::radians(360.9856));

	resource_manager* resource_manager = ctx->resource_manager;
	entt::registry& ecs_registry = *ctx->ecs_registry;
	
	ctx->sun_direct->set_intensity(1.0f);
	ctx->sun_direct->set_color({1, 1, 1});
	
	
	// Create sun
	{
		ecs::orbit_component sun_orbit;
		sun_orbit.elements.a = 1.0;
		sun_orbit.elements.ec = 0.016709;
		sun_orbit.elements.w = math::radians(282.9404);
		sun_orbit.elements.ma = math::radians(356.0470);
		sun_orbit.elements.i = 0.0;
		sun_orbit.elements.om = 0.0;
		
		sun_orbit.rate.a = 0.0;
		sun_orbit.rate.ec = -1.151e-9;
		sun_orbit.rate.w = math::radians(4.70935e-5);
		sun_orbit.rate.ma = math::radians(0.9856002585);
		sun_orbit.rate.i = 0.0;
		sun_orbit.rate.om = 0.0;
		
		ecs::transform_component sun_transform;
		sun_transform.local = math::identity_transform<float>;
		sun_transform.warp = true;
		
		auto sun_entity = ecs_registry.create();
		ecs_registry.assign<ecs::transform_component>(sun_entity, sun_transform);
		ecs_registry.assign<ecs::orbit_component>(sun_entity, sun_orbit);
		
		ctx->astronomy_system->set_sun(sun_entity);
	}
	
	// Create moon
	{

		ecs::orbit_component moon_orbit;
		
		moon_orbit.elements.a = 0.00256955529;
		moon_orbit.elements.ec = 0.0554;
		moon_orbit.elements.w = math::radians(318.15);
		moon_orbit.elements.ma = math::radians(135.27);
		moon_orbit.elements.i = math::radians(5.16);
		moon_orbit.elements.om = math::radians(125.08);
		
		moon_orbit.rate.a = 0.0;
		moon_orbit.rate.ec = 0.0;
		moon_orbit.rate.w = math::radians(0.1643573223); // Argument of periapsis precession period, P_w
		moon_orbit.rate.ma = math::radians(13.176358); // Longitude rate, n
		moon_orbit.rate.i = 0.0;
		moon_orbit.rate.om = math::radians(-18.6 / 365.2422); // Longitude of the ascending node precession period, P_node
		
		ecs::transform_component moon_transform;
		moon_transform.local = math::identity_transform<float>;
		moon_transform.warp = true;
		
		auto moon_entity = ecs_registry.create();
		ecs_registry.assign<ecs::transform_component>(moon_entity, moon_transform);
		ecs_registry.assign<ecs::orbit_component>(moon_entity, moon_orbit);
		
		ctx->astronomy_system->set_moon(moon_entity);
	}

	// Load entity archetypes
	ecs::archetype* ant_hill_archetype = resource_manager->load<ecs::archetype>("ant-hill.ent");
	ecs::archetype* maple_tree_archetype = resource_manager->load<ecs::archetype>("maple-tree.ent");
	ecs::archetype* nest_archetype = resource_manager->load<ecs::archetype>("harvester-nest.ent");
	ecs::archetype* samara_archetype = resource_manager->load<ecs::archetype>("samara.ent");
	ecs::archetype* forceps_archetype = resource_manager->load<ecs::archetype>("forceps.ent");
	ecs::archetype* lens_archetype = resource_manager->load<ecs::archetype>("lens.ent");
	ecs::archetype* brush_archetype = resource_manager->load<ecs::archetype>("brush.ent");
	ecs::archetype* marker_archetype = resource_manager->load<ecs::archetype>("marker.ent");
	ecs::archetype* container_archetype = resource_manager->load<ecs::archetype>("container.ent");
	ecs::archetype* twig_archetype = resource_manager->load<ecs::archetype>("twig.ent");
	ecs::archetype* larva_archetype = resource_manager->load<ecs::archetype>("larva.ent");
	ecs::archetype* pebble_archetype = resource_manager->load<ecs::archetype>("pebble.ent");
	ecs::archetype* flashlight_archetype = resource_manager->load<ecs::archetype>("flashlight.ent");
	ecs::archetype* flashlight_light_cone_archetype = resource_manager->load<ecs::archetype>("flashlight-light-cone.ent");
	ecs::archetype* lens_light_cone_archetype = resource_manager->load<ecs::archetype>("lens-light-cone.ent");
	ecs::archetype* ant_head_archetype = resource_manager->load<ecs::archetype>("ant-head.ent");
	ecs::archetype* dandelion_plant_archetype = resource_manager->load<ecs::archetype>("dandelion-plant.ent");
	ecs::archetype* grassland_road_archetype = resource_manager->load<ecs::archetype>("grassland-road.ent");
	
	// Create tools
	forceps_archetype->assign(ecs_registry, ctx->forceps_entity);
	lens_archetype->assign(ecs_registry, ctx->lens_entity);
	brush_archetype->assign(ecs_registry, ctx->brush_entity);
	marker_archetype->assign(ecs_registry, ctx->marker_entity);
	container_archetype->assign(ecs_registry, ctx->container_entity);
	twig_archetype->assign(ecs_registry, ctx->twig_entity);
	
	// Create flashlight and light cone, set light cone parent to flashlight, and move both to underworld scene
	flashlight_archetype->assign(ecs_registry, ctx->flashlight_entity);
	auto flashlight_light_cone = flashlight_light_cone_archetype->create(ecs_registry);
	ecs::command::parent(ecs_registry, flashlight_light_cone, ctx->flashlight_entity);
	ecs::command::assign_render_layers(ecs_registry, ctx->flashlight_entity, 2);
	
	// Make lens tool's model instance unculled, so its shadow is always visible.
	scene::model_instance* lens_model_instance = ctx->render_system->get_model_instance(ctx->lens_entity);
	if (lens_model_instance)
	{
		lens_model_instance->set_culling_mask(&ctx->no_cull);
	}
	
	// Create lens light cone and set its parent to lens
	auto lens_light_cone = lens_light_cone_archetype->create(ecs_registry);
	//ecs::command::bind_transform(ecs_registry, lens_light_cone, ctx->lens_entity);
	ecs::command::parent(ecs_registry, lens_light_cone, ctx->lens_entity);
	
	
	
	// Hide inactive tools
	ecs::command::assign_render_layers(ecs_registry, ctx->forceps_entity, 0);
	ecs::command::assign_render_layers(ecs_registry, ctx->brush_entity, 0);
	ecs::command::assign_render_layers(ecs_registry, ctx->lens_entity, 0);
	ecs::command::assign_render_layers(ecs_registry, ctx->marker_entity, 0);
	ecs::command::assign_render_layers(ecs_registry, ctx->container_entity, 0);
	ecs::command::assign_render_layers(ecs_registry, ctx->twig_entity, 0);
	
	// Activate brush tool
	ctx->tool_system->set_active_tool(ctx->brush_entity);
	
	// Create background
	for (int i = 0; i < 4; ++i)
	{
		auto road_entity = grassland_road_archetype->create(ecs_registry);
		auto& transform = ecs_registry.get<ecs::transform_component>(road_entity);
		
		math::quaternion<float> rotation = math::angle_axis(math::half_pi<float> * static_cast<float>(i), float3{0, 1, 0});
		float3 translation = rotation * float3{0, 0, 1600.0f};
		
		transform.local = math::identity_transform<float>;
		transform.local.rotation = rotation;	
		transform.local.translation = translation;	
	}

	// Create ant-hill
	auto ant_hill_entity = ant_hill_archetype->create(ecs_registry);
	ecs::command::place(ecs_registry, ant_hill_entity, {0, 0});
	
	// Generate pebbles
	float pebble_radius = 300.0f;
	int pebble_count = 20;
	for (int i = 0; i < pebble_count; ++i)
	{
		float x = math::random(-pebble_radius, pebble_radius);
		float z = math::random(-pebble_radius, pebble_radius);
		
		auto pebble_entity = ant_head_archetype->create(ecs_registry);
		
		auto& transform = ecs_registry.get<ecs::transform_component>(pebble_entity);
		transform.local = math::identity_transform<float>;
		transform.local.rotation = math::angle_axis(math::random(0.0f, math::two_pi<float>), {0, 1, 0});
		transform.local.scale = float3{1, 1, 1} * math::random(0.75f, 1.25f);
		
		ecs::command::place(ecs_registry, pebble_entity, {x, z});
	}

	// Create maple tree
	//auto maple_tree_entity = maple_tree_archetype->create(ecs_registry);
	//ecs::command::place(ecs_registry, maple_tree_entity, {300, 200});

	// Creat nest
	auto nest_entity = nest_archetype->create(ecs_registry);

	// Create terrain
	int terrain_radius = 6;
	for (int x = -terrain_radius; x <= terrain_radius; ++x)
	{
		for (int z = -terrain_radius; z <= terrain_radius; ++z)
		{
			ecs::terrain_component terrain_component;
			terrain_component.subdivisions = TERRAIN_PATCH_RESOLUTION;
			terrain_component.x = x;
			terrain_component.z = z;
			auto terrain_entity = ecs_registry.create();
			ecs_registry.assign<ecs::terrain_component>(terrain_entity, terrain_component);
		}
	}

	// Create samaras
	for (int i = 0; i < 15; ++i)
	{
		auto samara_entity = samara_archetype->create(ecs_registry);

		auto& transform = ecs_registry.get<ecs::transform_component>(samara_entity);
		float zone = 200.0f;
		transform.local = math::identity_transform<float>;
		transform.local.translation.x = math::random(-zone, zone);
		transform.local.translation.y = math::random(50.0f, 150.0f);
		transform.local.translation.z = math::random(-zone, zone);

		ecs::samara_component samara_component;
		samara_component.angle = math::random(0.0f, math::radians(360.0f));
		samara_component.direction = math::normalize(float3{math::random(-1.0f, 1.0f), math::random(-1.0f, -5.0f), math::random(-1.0f, 1.0f)});
		samara_component.chirality = (math::random(0.0f, 1.0f) < 0.5f) ? -1.0f : 1.0f;

		ecs_registry.assign_or_replace<ecs::samara_component>(samara_entity, samara_component);
	}
	
	// Setup camera focal point
	ecs::transform_component focal_point_transform;
	focal_point_transform.local = math::identity_transform<float>;
	focal_point_transform.warp = true;
	ecs::camera_follow_component focal_point_follow;
	ecs::snap_component focal_point_snap;
	focal_point_snap.ray = {float3{0, 10000, 0}, float3{0, -1, 0}};
	focal_point_snap.warp = false;
	focal_point_snap.relative = true;
	focal_point_snap.autoremove = false;
	ecs_registry.assign_or_replace<ecs::transform_component>(ctx->focal_point_entity, focal_point_transform);
	ecs_registry.assign_or_replace<ecs::camera_follow_component>(ctx->focal_point_entity, focal_point_follow);
	ecs_registry.assign_or_replace<ecs::snap_component>(ctx->focal_point_entity, focal_point_snap);
	
	// Setup camera
	ctx->overworld_camera->look_at({0, 0, 1}, {0, 0, 0}, {0, 1, 0});
	ctx->camera_system->set_camera(ctx->overworld_camera);
	
	auto ant_head = ant_head_archetype->create(ecs_registry);
	ecs::command::place(ecs_registry, ant_head, {50, 0});
	
	ctx->overworld_scene->update_tweens();
	
	// Allocate a nest
	nest* nest = new ::nest();
	
	// Setup initial nest parameters
	float tunnel_radius = 1.15f;
	nest->set_tunnel_radius(tunnel_radius);
	nest::shaft* central_shaft = nest->get_central_shaft();
	central_shaft->chirality = 1.0f;
	central_shaft->rotation = math::radians(0.0f);
	central_shaft->depth = {0.0f, 200.0f};
	central_shaft->radius = {15.0f, 15.0f};
	central_shaft->pitch = {40.0f, 40.0f};
	central_shaft->translation = {{{0.0f, 0.0f}, {0.0f, 0.0f}}};
	central_shaft->current_depth = 0.0f;
	for (std::size_t i = 0; i < 4; ++i)
	{
		nest::chamber chamber;
		chamber.shaft = central_shaft;
		chamber.depth = (i + 1) * 50.0f;
		chamber.rotation = math::radians(0.0f);
		chamber.inner_radius = 4.0f;
		chamber.outer_radius = 10.0f;
		central_shaft->chambers.push_back(chamber);
	}
	
	// Dig nest shafts
	float shift = 0.1f;
	for (int i = 0; i < 800; ++i)
	{
		ecs::cavity_component cavity;
		cavity.position = nest->extend_shaft(*nest->get_central_shaft());
		cavity.position += float3{math::random(-shift, shift), math::random(-shift, shift), math::random(-shift, shift)};
		cavity.radius = tunnel_radius * math::random(1.0f, 1.1f);

		ecs_registry.assign<ecs::cavity_component>(ecs_registry.create(), cavity);
	}

	// Dig nest chambers
	/*
	for (int i = 0; i < central_shaft->chambers.size(); ++i)
	{
		for (int j = 0; j < 150; ++j)
		{
			ecs::cavity_component cavity;
			cavity.position = nest->expand_chamber(central_shaft->chambers[i]);
			cavity.position += float3{math::random(-shift, shift), math::random(-shift, shift), math::random(-shift, shift)};
			cavity.radius = tunnel_radius * math::random(1.0f, 1.1f);

			ecs_registry.assign<ecs::cavity_component>(ecs_registry.create(), cavity);
		}
	}
	*/
	
	// Place larva in chamber
	{
		auto larva = larva_archetype->create(ecs_registry);
		ecs::command::assign_render_layers(ecs_registry, larva, 1);
		ecs::command::warp_to(ecs_registry, larva, {50, 0.1935f, 10});
		//auto& transform = ecs_registry.get<ecs::transform_component>(larva_entity);
		//transform.transform = math::identity_transform<float>;
		//transform.transform.translation = nest->get_shaft_position(*central_shaft, central_shaft->depth[1]);
		//transform.transform.translation.y -= 1.0f;
	}
	
	auto dandelion_plant = dandelion_plant_archetype->create(ecs_registry);
	ecs::command::place(ecs_registry, dandelion_plant, {55, -30});
	
	ecs::control_system* control_system = ctx->control_system;
	control_system->update(0.0, 0.0);
	control_system->set_nest(nest);
	
	// Start fade in
	ctx->fade_transition->transition(1.0f, true, ease<float>::in_quad);

	logger->pop_task(EXIT_SUCCESS);
	
	std::string biome_name = (*ctx->strings)[ctx->biome->name];
	logger->log("Entered biome \"" + biome_name + "\"");
	
	
	std::srand(std::time(nullptr));
	//auto rng = [](){ return std::rand(); };
	
	std::random_device rd;
	std::mt19937 rng(rd());
	
	
	std::string sequence_a = "CCTTGCCCTTTGGGTCGCCCCCCTAG";
	std::string sequence_b = "ATGTTTCCCGAAGGGTAG";
	std::string sequence_c = "AAATGCCCCCCCCCCCCCCCCCCCCCCCCCCCTAGAAAAAAAAA";
	std::string orf_a;
	std::string protein_a;
	std::string protein_b;
	std::string protein_c;
	
	
	std::cout << "sequence a: " << sequence_a << std::endl;
	genetics::sequence::transcribe(sequence_a.begin(), sequence_a.end(), sequence_a.begin());
	std::cout << "sequence a: " << sequence_a << std::endl;
	
	std::string complement;
	genetics::sequence::rna::complement(sequence_a.begin(), sequence_a.end(), std::back_inserter(complement));
	std::cout << "complement: "  << complement << std::endl;
	
	auto orf = genetics::sequence::find_orf(sequence_a.begin(), sequence_a.end(), genetics::standard_code);
	if (orf.start != sequence_a.end())
	{
		std::copy(orf.start, orf.stop, std::back_inserter(orf_a));
		std::cout << "orf      a: " << orf_a << std::endl;
		
		genetics::sequence::translate(orf.start, orf.stop, std::back_inserter(protein_a), genetics::standard_code);
		std::cout << "protein  a: " << protein_a << std::endl;
	}
	
	protein_b = "MFFFFP";
	protein_c = "MFFFYP";
	int score;

	std::cout << std::endl;
	std::cout << "protein_b: " << protein_b << std::endl;
	std::cout << "protein_c: " << protein_c << std::endl;
	
	score = genetics::protein::score(protein_b.begin(), protein_b.end(), protein_c.begin(), genetics::matrix::blosum62<int>);
	std::cout << "score blosum62: " << score << std::endl;
	
	score = genetics::protein::score(protein_b.begin(), protein_b.end(), protein_c.begin(), genetics::matrix::blosum80<int>);
	std::cout << "score blosum80: " << score << std::endl;
	
	
	std::cout << "identity  : " << genetics::protein::identity<float>(protein_b.begin(), protein_b.end(), protein_c.begin()) << std::endl;
	std::cout << "similarity62: " << genetics::protein::similarity<float>(protein_b.begin(), protein_b.end(), protein_c.begin(), genetics::matrix::blosum62<int>) << std::endl;
	std::cout << "similarity80: " << genetics::protein::similarity<float>(protein_b.begin(), protein_b.end(), protein_c.begin(), genetics::matrix::blosum80<int>) << std::endl;
	
}

void play_state_exit(game_context* ctx)
{
	debug::logger* logger = ctx->logger;
	logger->push_task("Exiting play state");
	
	logger->pop_task(EXIT_SUCCESS);
}

