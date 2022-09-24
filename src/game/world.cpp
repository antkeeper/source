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

#include "game/world.hpp"
#include "scene/text.hpp"
#include "physics/light/vmag.hpp"
#include "color/color.hpp"
#include "entity/components/atmosphere.hpp"
#include "entity/components/blackbody.hpp"
#include "entity/components/celestial-body.hpp"
#include "entity/components/orbit.hpp"
#include "entity/components/terrain.hpp"
#include "entity/components/transform.hpp"
#include "entity/systems/astronomy.hpp"
#include "entity/systems/orbit.hpp"
#include "entity/commands.hpp"
#include "entity/archetype.hpp"
#include "geom/spherical.hpp"
#include "gl/drawing-mode.hpp"
#include "gl/vertex-array.hpp"
#include "gl/vertex-attribute.hpp"
#include "gl/vertex-buffer.hpp"
#include "physics/light/photometry.hpp"
#include "physics/orbit/orbit.hpp"
#include "physics/orbit/ephemeris.hpp"
#include "physics/time/gregorian.hpp"
#include "physics/time/constants.hpp"
#include "render/material.hpp"
#include "render/model.hpp"
#include "render/passes/shadow-map-pass.hpp"
#include "render/vertex-attribute.hpp"
#include "resources/resource-manager.hpp"
#include "scene/ambient-light.hpp"
#include "scene/directional-light.hpp"
#include "gl/texture-wrapping.hpp"
#include "gl/texture-filter.hpp"
#include "render/material-flags.hpp"
#include "config.hpp"
#include <iostream>

namespace game {
namespace world {

/// Loads an ephemeris.
static void load_ephemeris(game::context& ctx);

/// Creates the fixed stars.
static void create_stars(game::context& ctx);

/// Creates the Sun.
static void create_sun(game::context& ctx);

/// Creates the Earth-Moon system.
static void create_earth_moon_system(game::context& ctx);

/// Creates the Earth.
static void create_earth(game::context& ctx);

/// Creates the Moon.
static void create_moon(game::context& ctx);

void cosmogenesis(game::context& ctx)
{
	ctx.logger->push_task("Generating cosmos");
	
	load_ephemeris(ctx);
	create_stars(ctx);
	create_sun(ctx);
	create_earth_moon_system(ctx);
	
	ctx.logger->pop_task(EXIT_SUCCESS);
}

void set_location(game::context& ctx, double elevation, double latitude, double longitude)
{
	// Update context location
	ctx.elevation = elevation;
	ctx.latitude = latitude;
	ctx.longitude = longitude;
	
	// Pass location to astronomy system
	ctx.astronomy_system->set_observer_location({elevation, latitude, longitude});
}

void set_time(game::context& ctx, double t)
{
	ctx.logger->push_task("Setting time to UT1 " + std::to_string(t));
	try
	{
		ctx.astronomy_system->set_time(t);
		ctx.orbit_system->set_time(t);
	}
	catch (const std::exception&)
	{
		ctx.logger->pop_task(EXIT_FAILURE);
		return;
	}
	ctx.logger->pop_task(EXIT_SUCCESS);
}

void set_time(game::context& ctx, int year, int month, int day, int hour, int minute, double second)
{
	const double utc_offset = ctx.longitude / (math::two_pi<double> / 24.0);
	const double t = physics::time::gregorian::to_ut1<double>(year, month, day, hour, minute, second, utc_offset);
	set_time(ctx, t);
}

void set_time_scale(game::context& ctx, double scale)
{
	ctx.logger->push_task("Setting time scale to " + std::to_string(scale));
	try
	{
		// Convert time scale from seconds to days
		const double astronomical_scale = scale / physics::time::seconds_per_day<double>;
		
		ctx.orbit_system->set_time_scale(astronomical_scale);
		ctx.astronomy_system->set_time_scale(astronomical_scale);
	}
	catch (const std::exception&)
	{
		ctx.logger->pop_task(EXIT_FAILURE);
		return;
	}
	ctx.logger->pop_task(EXIT_SUCCESS);
}

void load_ephemeris(game::context& ctx)
{
	ctx.logger->push_task("Loading ephemeris");
	
	try
	{
		std::string ephemeris_filename;
		if (ctx.config->contains("ephemeris"))
		{
			ephemeris_filename = (*ctx.config)["ephemeris"].get<std::string>();
		}
		else
		{
			ctx.logger->warning("No ephemeris set in config");
			ctx.logger->pop_task(EXIT_FAILURE);
			return;
		}
		
		ctx.orbit_system->set_ephemeris(ctx.resource_manager->load<physics::orbit::ephemeris<double>>(ephemeris_filename));
	}
	catch (const std::exception&)
	{
		ctx.logger->pop_task(EXIT_FAILURE);
		return;
	}
	
	ctx.logger->pop_task(EXIT_SUCCESS);
}

void create_stars(game::context& ctx)
{
	ctx.logger->push_task("Generating fixed stars");
	
	// Load star catalog
	string_table* star_catalog = nullptr;
	try
	{
		std::string star_catalog_filename;
		if (ctx.config->contains("star_catalog"))
		{
			star_catalog_filename = (*ctx.config)["star_catalog"].get<std::string>();
		}
		else
		{
			ctx.logger->warning("No star catalog set in config");
			ctx.logger->pop_task(EXIT_FAILURE);
			return;
		}
		
		star_catalog = ctx.resource_manager->load<string_table>(star_catalog_filename);
	}
	catch (const std::exception&)
	{
		ctx.logger->pop_task(EXIT_FAILURE);
		return;
	}
	
	// Allocate star catalog vertex data
	std::size_t star_count = 0;
	if (star_catalog->size() > 0)
		star_count = star_catalog->size() - 1;
	std::size_t star_vertex_size = 7;
	std::size_t star_vertex_stride = star_vertex_size * sizeof(float);
	float* star_vertex_data = new float[star_count * star_vertex_size];
	float* star_vertex = star_vertex_data;
	
	// Init starlight illuminance
	double starlight_illuminance = 0.0;
	
	// Build star catalog vertex data
	for (std::size_t i = 1; i < star_catalog->size(); ++i)
	{
		const string_table_row& catalog_row = (*star_catalog)[i];
		
		// Parse star catalog item
		double ra = 0.0;
		double dec = 0.0;
		double vmag = 0.0;
		double bv = 0.0;
		try
		{
			ra = std::stod(catalog_row[1]);
			dec = std::stod(catalog_row[2]);
			vmag = std::stod(catalog_row[3]);
			bv = std::stod(catalog_row[4]);
		}
		catch (const std::exception&)
		{
			ctx.logger->warning("Invalid star catalog item on row " + std::to_string(i));
		}
		
		// Convert right ascension and declination from degrees to radians
		ra = math::wrap_radians(math::radians(ra));
		dec = math::wrap_radians(math::radians(dec));
		
		// Convert ICRF coordinates from spherical to Cartesian
		double3 position = physics::orbit::frame::bci::cartesian(double3{1.0, dec, ra});
		
		// Convert apparent magnitude to brightness factor relative to a 0th magnitude star
		double brightness = physics::light::vmag::to_brightness(vmag);
		
		// Convert color index to color temperature
		double cct = color::index::bv_to_cct(bv);
		
		// Calculate XYZ color from color temperature
		double3 color_xyz = color::cct::to_xyz(cct);
		
		// Transform XYZ color to ACEScg colorspace
		double3 color_acescg = color::xyz::to_acescg(color_xyz);
		
		// Scale color by relative brightness
		color_acescg *= brightness;
		
		// Build vertex
		*(star_vertex++) = static_cast<float>(position.x);
		*(star_vertex++) = static_cast<float>(position.y);
		*(star_vertex++) = static_cast<float>(position.z);
		*(star_vertex++) = static_cast<float>(color_acescg.x);
		*(star_vertex++) = static_cast<float>(color_acescg.y);
		*(star_vertex++) = static_cast<float>(color_acescg.z);
		*(star_vertex++) = static_cast<float>(brightness);
		
		// Convert apparent magnitude to illuminance and add to total starlight
		starlight_illuminance += physics::light::vmag::to_illuminance(vmag);
	}
	
	// Unload star catalog
	ctx.resource_manager->unload("stars.csv");
	
	// Allocate stars model
	render::model* stars_model = new render::model();
	
	// Get model VBO and VAO
	gl::vertex_buffer* vbo = stars_model->get_vertex_buffer();
	gl::vertex_array* vao = stars_model->get_vertex_array();
	
	// Resize model VBO and upload vertex data
	vbo->resize(star_count * star_vertex_stride, star_vertex_data);
	
	// Free star catalog vertex data
	delete[] star_vertex_data;
	
	std::size_t attribute_offset = 0;
	
	// Define position vertex attribute
	gl::vertex_attribute position_attribute;
	position_attribute.buffer = vbo;
	position_attribute.offset = attribute_offset;
	position_attribute.stride = star_vertex_stride;
	position_attribute.type = gl::vertex_attribute_type::float_32;
	position_attribute.components = 3;
	attribute_offset += position_attribute.components * sizeof(float);
	
	// Define color vertex attribute
	gl::vertex_attribute color_attribute;
	color_attribute.buffer = vbo;
	color_attribute.offset = attribute_offset;
	color_attribute.stride = star_vertex_stride;
	color_attribute.type = gl::vertex_attribute_type::float_32;
	color_attribute.components = 4;
	attribute_offset += color_attribute.components * sizeof(float);
	
	// Bind vertex attributes to VAO
	vao->bind(render::vertex_attribute::position, position_attribute);
	vao->bind(render::vertex_attribute::color, color_attribute);
	
	// Load star material
	render::material* star_material = ctx.resource_manager->load<render::material>("fixed-star.mtl");
	
	// Create model group
	render::model_group* stars_model_group = stars_model->add_group("stars");
	stars_model_group->set_material(star_material);
	stars_model_group->set_drawing_mode(gl::drawing_mode::points);
	stars_model_group->set_start_index(0);
	stars_model_group->set_index_count(star_count);
	
	// Pass stars model to sky pass
	ctx.sky_pass->set_stars_model(stars_model);
	
	// Pass starlight illuminance to astronomy system
	ctx.astronomy_system->set_starlight_illuminance(starlight_illuminance);
	
	ctx.logger->pop_task(EXIT_SUCCESS);
}

void create_sun(game::context& ctx)
{
	ctx.logger->push_task("Generating Sun");
	
	try
	{
		// Create sun entity
		entity::archetype* sun_archetype = ctx.resource_manager->load<entity::archetype>("sun.ent");
		entity::id sun_eid = sun_archetype->create(*ctx.entity_registry);
		ctx.entities["sun"] = sun_eid;
		
		// Create sun directional light scene object
		scene::directional_light* sun_light = new scene::directional_light();
		sun_light->set_color({0, 0, 0});
		sun_light->update_tweens();
		
		// Create sky ambient light scene object
		scene::ambient_light* sky_light = new scene::ambient_light();
		sky_light->set_color({0, 0, 0});
		sky_light->update_tweens();
		
		// Add sun light scene objects to surface scene
		ctx.surface_scene->add_object(sun_light);
		ctx.surface_scene->add_object(sky_light);
		
		// Pass direct sun light scene object to shadow map pass and astronomy system
		ctx.surface_shadow_map_pass->set_light(sun_light);
		ctx.astronomy_system->set_sun_light(sun_light);
		ctx.astronomy_system->set_sky_light(sky_light);
	}
	catch (const std::exception&)
	{
		ctx.logger->pop_task(EXIT_FAILURE);
		return;
	}
	
	ctx.logger->pop_task(EXIT_SUCCESS);
}

void create_earth_moon_system(game::context& ctx)
{
	ctx.logger->push_task("Generating Earth-Moon system");
	
	try
	{
		// Create Earth-Moon barycenter entity
		entity::archetype* em_bary_archetype = ctx.resource_manager->load<entity::archetype>("em-bary.ent");
		entity::id em_bary_eid = em_bary_archetype->create(*ctx.entity_registry);
		ctx.entities["em_bary"] = em_bary_eid;
		
		// Create Earth
		create_earth(ctx);
		
		// Create Moon
		create_moon(ctx);
	}
	catch (const std::exception&)
	{
		ctx.logger->pop_task(EXIT_FAILURE);
		return;
	}
	
	ctx.logger->pop_task(EXIT_SUCCESS);
}

void create_earth(game::context& ctx)
{
	ctx.logger->push_task("Generating Earth");
	
	try
	{
		// Create earth entity
		entity::archetype* earth_archetype = ctx.resource_manager->load<entity::archetype>("earth.ent");
		entity::id earth_eid = earth_archetype->create(*ctx.entity_registry);
		ctx.entities["earth"] = earth_eid;
		
		// Assign orbital parent
		ctx.entity_registry->get<entity::component::orbit>(earth_eid).parent = ctx.entities["em_bary"];
		
		// Assign earth terrain component
		entity::component::terrain terrain;
		terrain.elevation = [](double, double) -> double
		{
			//return math::random<double>(0.0, 1.0);
			return 0.0;
		};
		terrain.max_lod = 0;
		terrain.patch_material = nullptr;
		//ctx.entity_registry->assign<entity::component::terrain>(earth_eid, terrain);
		
		// Pass earth to astronomy system as reference body
		ctx.astronomy_system->set_reference_body(earth_eid);
	}
	catch (const std::exception&)
	{
		ctx.logger->pop_task(EXIT_FAILURE);
		return;
	}
	
	ctx.logger->pop_task(EXIT_SUCCESS);
}

void create_moon(game::context& ctx)
{
	ctx.logger->push_task("Generating Moon");
	
	try
	{
		// Create lunar entity
		entity::archetype* moon_archetype = ctx.resource_manager->load<entity::archetype>("moon.ent");
		entity::id moon_eid = moon_archetype->create(*ctx.entity_registry);
		ctx.entities["moon"] = moon_eid;
		
		// Assign orbital parent
		ctx.entity_registry->get<entity::component::orbit>(moon_eid).parent = ctx.entities["em_bary"];
		
		// Pass moon model to sky pass
		ctx.sky_pass->set_moon_model(ctx.resource_manager->load<render::model>("moon.mdl"));
		
		// Create moon directional light scene object
		scene::directional_light* moon_light = new scene::directional_light();
		moon_light->set_color({0, 0, 0});
		moon_light->update_tweens();
		
		// Add moon light scene objects to surface scene
		ctx.surface_scene->add_object(moon_light);
		
		// Pass moon light scene object to astronomy system
		ctx.astronomy_system->set_moon_light(moon_light);
	}
	catch (const std::exception&)
	{
		ctx.logger->pop_task(EXIT_FAILURE);
		return;
	}
	
	ctx.logger->pop_task(EXIT_SUCCESS);
}

} // namespace world
} // namespace game
