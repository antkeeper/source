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

#include "game/state/credits.hpp"
#include "game/state/extras-menu.hpp"
#include "game/context.hpp"
#include "animation/ease.hpp"
#include "animation/animation.hpp"
#include "animation/animator.hpp"
#include "application.hpp"
#include "scene/text.hpp"
#include "debug/logger.hpp"

namespace game {
namespace state {

credits::credits(game::context& ctx):
	game::state::base(ctx)
{
	ctx.logger->push_task("Entering credits state");
	
	// Construct credits text
	credits_text.set_material(&ctx.menu_font_material);
	credits_text.set_font(&ctx.menu_font);
	credits_text.set_color({1.0f, 1.0f, 1.0f, 0.0f});
	credits_text.set_content((*ctx.strings)["credits"]);
	
	// Align credits text
	const auto& credits_aabb = static_cast<const geom::aabb<float>&>(credits_text.get_local_bounds());
	float credits_w = credits_aabb.max_point.x - credits_aabb.min_point.x;
	float credits_h = credits_aabb.max_point.y - credits_aabb.min_point.y;
	credits_text.set_translation({std::round(-credits_w * 0.5f), std::round(-credits_h * 0.5f), 0.0f});
	credits_text.update_tweens();
	
	// Load animation timing configuration
	double credits_fade_in_duration = 0.0;
	double credits_scroll_duration = 0.0;
	if (ctx.config->contains("credits_fade_in_duration"))
		credits_fade_in_duration = (*ctx.config)["credits_fade_in_duration"].get<double>();
	if (ctx.config->contains("credits_scroll_duration"))
		credits_scroll_duration = (*ctx.config)["credits_scroll_duration"].get<double>();
	
	auto set_credits_opacity = [this](int channel, const float& opacity)
	{
		this->credits_text.set_color({1.0f, 1.0f, 1.0f, opacity});
	};
	
	// Build credits fade in animation
	credits_fade_in_animation.set_interpolator(ease<float>::in_quad);
	animation_channel<float>* credits_fade_in_opacity_channel = credits_fade_in_animation.add_channel(0);
	credits_fade_in_opacity_channel->insert_keyframe({0.0, 0.0f});
	credits_fade_in_opacity_channel->insert_keyframe({credits_fade_in_duration, 1.0f});
	credits_fade_in_animation.set_frame_callback(set_credits_opacity);
	
	// Add credits animations to animator
	ctx.animator->add_animation(&credits_fade_in_animation);
	
	// Start credits fade in animation
	credits_fade_in_animation.play();
	
	// Set up credits skipper
	ctx.input_listener->set_callback
	(
		[this, &ctx](const event_base& event)
		{
			auto id = event.get_event_type_id();
			if (id != mouse_moved_event::event_type_id && id != mouse_wheel_scrolled_event::event_type_id && id != gamepad_axis_moved_event::event_type_id)
			{
				if (this->credits_text.get_color()[3] > 0.0f)
				{
					ctx.input_listener->set_enabled(false);
					
					// Change state
					ctx.state_machine.pop();
					ctx.state_machine.emplace(new game::state::extras_menu(ctx));
				}
			}
		}
	);
	ctx.input_listener->set_enabled(true);
	
	ctx.ui_scene->add_object(&credits_text);
	
	ctx.logger->pop_task(EXIT_SUCCESS);
}

credits::~credits()
{
	ctx.logger->push_task("Exiting credits state");
	
	// Disable credits skipper
	ctx.input_listener->set_enabled(false);
	ctx.input_listener->set_callback(nullptr);
	
	// Destruct credits text
	ctx.ui_scene->remove_object(&credits_text);
	
	// Destruct credits animations
	ctx.animator->remove_animation(&credits_fade_in_animation);
	
	ctx.logger->pop_task(EXIT_SUCCESS);
}

} // namespace state
} // namespace game