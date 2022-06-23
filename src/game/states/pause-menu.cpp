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

#include "game/states/pause-menu.hpp"
#include "game/states/main-menu.hpp"
#include "game/states/options-menu.hpp"
#include "game/menu.hpp"
#include "animation/ease.hpp"
#include "animation/animation.hpp"
#include "animation/animator.hpp"
#include "application.hpp"
#include "scene/text.hpp"

namespace game {
namespace state {
namespace pause_menu {

void enter(game::context* ctx)
{
	// Construct menu item texts
	scene::text* resume_text = new scene::text();
	scene::text* options_text = new scene::text();
	scene::text* main_menu_text = new scene::text();
	scene::text* quit_text = new scene::text();
	
	// Set content of menu item texts
	resume_text->set_content((*ctx->strings)["pause_menu_resume"]);
	options_text->set_content((*ctx->strings)["pause_menu_options"]);
	main_menu_text->set_content((*ctx->strings)["pause_menu_main_menu"]);
	quit_text->set_content((*ctx->strings)["pause_menu_quit"]);
	
	// Build list of menu item texts
	ctx->menu_item_texts.push_back({resume_text, nullptr});
	ctx->menu_item_texts.push_back({options_text, nullptr});
	ctx->menu_item_texts.push_back({main_menu_text, nullptr});
	ctx->menu_item_texts.push_back({quit_text, nullptr});
	
	// Init menu item index
	game::menu::init_menu_item_index(ctx, "pause");
	
	game::menu::update_text_color(ctx);
	game::menu::update_text_font(ctx);
	game::menu::align_text(ctx, true, false);
	game::menu::update_text_tweens(ctx);
	game::menu::add_text_to_ui(ctx);
	game::menu::setup_animations(ctx);
	
	// Construct menu item callbacks
	auto select_resume_callback = [ctx]()
	{
		// Disable unpause control
		ctx->controls["pause"]->set_activated_callback(nullptr);
		
		// Disable menu controls
		game::menu::clear_controls(ctx);
		
		auto resume_paused_state = [ctx]()
		{
			ctx->app->queue_state(*ctx->paused_state);
		};
		
		// Fade out pause menu then resume paused state
		game::menu::fade_out(ctx, resume_paused_state);
		game::menu::fade_out_bg(ctx);
	};
	auto select_options_callback = [ctx]()
	{
		// Disable unpause control
		ctx->controls["pause"]->set_activated_callback(nullptr);
		
		// Disable menu controls
		game::menu::clear_controls(ctx);
		
		// Fade out pause menu then open options menu
		game::menu::fade_out
		(
			ctx,
			[ctx]()
			{
				application::state next_state;
				next_state.name = "options_menu";
				next_state.enter = std::bind(game::state::options_menu::enter, ctx);
				next_state.exit = std::bind(game::state::options_menu::exit, ctx);
				ctx->app->queue_state(next_state);
			}
		);
	};
	auto select_main_menu_callback = [ctx]()
	{
		// Disable unpause control
		ctx->controls["pause"]->set_activated_callback(nullptr);
		
		// Disable menu controls
		game::menu::clear_controls(ctx);
		
		// Clear paused state
		ctx->paused_state.reset();
		
		// Fade out pause menu then return to main menu
		game::menu::fade_out
		(
			ctx,
			[ctx]()
			{
				application::state next_state;
				next_state.name = "main_menu";
				next_state.enter = std::bind(game::state::main_menu::enter, ctx, true);
				next_state.exit = std::bind(game::state::main_menu::exit, ctx);
				ctx->app->queue_state(next_state);
			}
		);
	};
	auto select_quit_callback = [ctx]()
	{
		// Disable unpause control
		ctx->controls["pause"]->set_activated_callback(nullptr);
		
		// Disable menu controls
		game::menu::clear_controls(ctx);
		
		// Clear paused state
		ctx->paused_state.reset();
		
		// Fade out then quit
		game::menu::fade_out(ctx, std::bind(&application::close, ctx->app, EXIT_SUCCESS));
	};
	
	// Build list of menu select callbacks
	ctx->menu_select_callbacks.push_back(select_resume_callback);
	ctx->menu_select_callbacks.push_back(select_options_callback);
	ctx->menu_select_callbacks.push_back(select_main_menu_callback);
	ctx->menu_select_callbacks.push_back(select_quit_callback);
	
	// Build list of menu right callbacks
	ctx->menu_right_callbacks.resize(4, nullptr);
	
	// Build list of menu left callbacks
	ctx->menu_left_callbacks.resize(4, nullptr);
	
	// Set menu back callback
	ctx->menu_back_callback = select_resume_callback;
	
	// Queue control setup
	ctx->function_queue.push
	(
		[ctx, select_resume_callback]()
		{
			// Enable unpause control
			ctx->controls["pause"]->set_activated_callback(select_resume_callback);
			
			// Enable menu controls
			game::menu::setup_controls(ctx);
		}
	);
	
	// Fade in menu and menu BG
	game::menu::fade_in(ctx, nullptr);
	if (!ctx->menu_bg_billboard->is_active())
		game::menu::fade_in_bg(ctx);
}

void exit(game::context* ctx)
{
	// Destruct menu
	game::menu::clear_controls(ctx);
	game::menu::clear_callbacks(ctx);
	game::menu::delete_animations(ctx);
	game::menu::remove_text_from_ui(ctx);
	game::menu::delete_text(ctx);
}

} // namespace pause_menu
} // namespace state
} // namespace game
