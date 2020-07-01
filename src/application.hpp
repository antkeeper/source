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

#ifndef ANTKEEPER_APPLICATION_HPP
#define ANTKEEPER_APPLICATION_HPP

// STL
#include <fstream>
#include <functional>
#include <list>
#include <map>
#include <string>

// External
#include <entt/entt.hpp>

// Debug
#include "debug/logger.hpp"
#include "debug/performance-sampler.hpp"

// Input
#include "input/control.hpp"
#include "input/control-set.hpp"
#include "input/keyboard.hpp"
#include "input/mouse.hpp"
#include "input/game-controller.hpp"
#include "input/input-event-router.hpp"
#include "input/input-mapper.hpp"

// Event
#include "event/event-dispatcher.hpp"

// Renderer
#include "renderer/compositor.hpp"
#include "renderer/renderer.hpp"

// Scene
#include "scene/scene.hpp"
#include "scene/camera.hpp"
#include "scene/ambient-light.hpp"
#include "scene/directional-light.hpp"
#include "scene/point-light.hpp"
#include "scene/spotlight.hpp"
#include "scene/model-instance.hpp"

// Animation
#include "animation/timeline.hpp"
#include "animation/animation.hpp"
#include "animation/tween.hpp"

// Misc
#include "state/fsm.hpp"
#include "frame-scheduler.hpp"
#include "pheromone-matrix.hpp"
#include "orbit-cam.hpp"

// Forward declarations
//{
	// SDL
	typedef struct SDL_Window SDL_Window;
	typedef void* SDL_GLContext;
	
	// Resources
	class resource_manager;
	
	// Rasterizer
	class rasterizer;
	class framebuffer;
	class vertex_buffer;
	class vertex_array;
	class texture_2d;
	
	// Renderer
	class clear_pass;
	class shadow_map_pass;
	class material_pass;
	class sky_pass;
	class bloom_pass;
	class final_pass;

	// Systems
	class behavior_system;
	class camera_system;
	class collision_system;
	class locomotion_system;
	class model_system;
	class nest_system;
	class placement_system;
	class samara_system;
	class subterrain_system;
	class terrain_system;
	class vegetation_system;
	class tool_system;
	class control_system;
	class ui_system;
//}

class application
{
public:
	/**
	 * Creates and initializes an application.
	 */
	application(int argc, char** argv);
	
	/**
	 * Destroys an application.
	 */
	~application();

	/**
	 * Executes the application, causing it to enter the execution loop until closed.
	 *
	 * @return Exit status code.
	 */
	int execute();

	/**
	 * Requests the application's execution loop to cleanly terminate, and specifies its exit status code.
	 *
	 * @param status Status to be returned by application::execute() upon execution loop termination.
	 */
	void close(int status);

	logger* get_logger();
	resource_manager* get_resource_manager();
	fsm::machine* get_state_machine();
	const fsm::state& get_loading_state() const;
	const fsm::state& get_language_select_state() const;
	const fsm::state& get_splash_state() const;
	const fsm::state& get_title_state() const;
	const fsm::state& get_play_state() const;
	const fsm::state& get_pause_state() const;
	timeline* get_timeline();
	animator* get_animator();
	camera* get_camera();
	orbit_cam* get_orbit_cam();
	control_system* get_control_system();

	entt::registry& get_ecs_registry();
	scene& get_scene();

	void take_screenshot() const;

private:
	void update(double t, double dt);
	void render(double alpha);
	void translate_sdl_events();
	void set_relative_mouse_mode(bool enabled);
	void toggle_fullscreen();
	void window_resized();
	static void save_image(const std::string& filename, int w, int h, const unsigned char* pixels);

	bool fullscreen;
	std::tuple<int, int> saved_mouse_position;
	std::tuple<int, int> window_dimensions;
	std::tuple<int, int> window_position;
	std::tuple<int, int> display_dimensions;
	float4 viewport;

	// Debugging
	std::ofstream log_filestream;
	logger logger;

	// Paths
	std::string data_path;
	std::string config_path;
	std::string screenshots_path;

	// Resources
	resource_manager* resource_manager;

	SDL_Window* window;
	SDL_GLContext context;
	bool closed;
	int exit_status;

	// Updatable systems
	timeline timeline;
	animator animator;
	std::list<std::function<void(double, double)>> systems;
	
	int shadow_map_resolution;
	framebuffer* shadow_map_framebuffer;
	texture_2d* shadow_map_depth_texture;
	
	framebuffer* framebuffer_hdr;
	texture_2d* framebuffer_hdr_color;
	texture_2d* framebuffer_hdr_depth;
	
	framebuffer* framebuffer_bloom; // General purpose framebuffer A
	texture_2d* bloom_texture;
	
	// Rendering
	rasterizer* rasterizer;
	material* fallback_material;
	::clear_pass* clear_pass;
	::sky_pass* sky_pass;
	::material_pass* material_pass;
	compositor default_compositor;
	::clear_pass* shadow_map_clear_pass;
	::shadow_map_pass* shadow_map_pass;
	::bloom_pass* bloom_pass;
	::final_pass* final_pass;
	
	camera default_camera;
	ambient_light sun_indirect;
	directional_light sun_direct;
	point_light subterrain_light;
	ambient_light underworld_ambient_light;
	model_instance darkness_volume;
	model_instance lantern;
	model_instance cloud;
	model_instance* grass_patches;
	::spotlight spotlight;
	vertex_buffer* billboard_vbo;
	vertex_array* billboard_vao;
	::renderer renderer;
	scene overworld_scene;
	scene underworld_scene;
	scene* active_scene;

	// FSM
	fsm::machine state_machine;
	fsm::state loading_state;
	fsm::state language_select_state;
	fsm::state splash_state;
	fsm::state title_state;
	fsm::state play_state;
	fsm::state pause_state;

	// Frame timing
	frame_scheduler frame_scheduler;
	performance_sampler performance_sampler;
	tween<double> time;

	// Events
	event_dispatcher event_dispatcher;
	input_event_router input_event_router;
	input_mapper input_mapper;

	// Input devices
	keyboard keyboard;
	mouse mouse;
	game_controller game_controller;

	// Controls
	control_set menu_controls;
	control menu_back_control;
	control menu_select_control;
	control_set* camera_controls;

	// System controls
	control_set application_controls;
	control toggle_fullscreen_control;
	control screenshot_control;
	control dig_control;

	// Game
	orbit_cam orbit_cam;
	pheromone_matrix pheromones;
	control_system* control_system;

	// ECS
	entt::registry ecs_registry;
	behavior_system* behavior_system;
	camera_system* camera_system;
	collision_system* collision_system;
	locomotion_system* locomotion_system;
	model_system* model_system;
	nest_system* nest_system;
	placement_system* placement_system;
	samara_system* samara_system;
	subterrain_system* subterrain_system;
	terrain_system* terrain_system;
	vegetation_system* vegetation_system;
	tool_system* tool_system;
	
	// UI
	ui_system* ui_system;
	compositor ui_compositor;
	::clear_pass* ui_clear_pass;
	::material_pass* ui_material_pass;
	
	// Animation
	tween<float3> focal_point_tween;
};

inline logger* application::get_logger()
{
	return &logger;
}

inline resource_manager* application::get_resource_manager()
{
	return resource_manager;
}

inline fsm::machine* application::get_state_machine()
{
	return &state_machine;
}

inline const fsm::state& application::get_loading_state() const
{
	return loading_state;
}

inline const fsm::state& application::get_language_select_state() const
{
	return language_select_state;
}

inline const fsm::state& application::get_splash_state() const
{
	return splash_state;
}

inline const fsm::state& application::get_title_state() const
{
	return title_state;
}

inline const fsm::state& application::get_play_state() const
{
	return play_state;
}

inline const fsm::state& application::get_pause_state() const
{
	return pause_state;
}

inline timeline* application::get_timeline()
{
	return &timeline;
}

inline animator* application::get_animator()
{
	return &animator;
}

inline camera* application::get_camera()
{
	return &default_camera;
}

inline orbit_cam* application::get_orbit_cam()
{
	return &orbit_cam;
}

inline control_system* application::get_control_system()
{
	return control_system;
}

inline entt::registry& application::get_ecs_registry()
{
	return ecs_registry;
}

inline scene& application::get_scene()
{
	return overworld_scene;
}

#endif // ANTKEEPER_APPLICATION_HPP
