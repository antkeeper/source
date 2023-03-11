/*
 * Copyright (C) 2023  Christopher J. Howard
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

#include <engine/render/passes/final-pass.hpp>
#include <engine/resources/resource-manager.hpp>
#include <engine/gl/rasterizer.hpp>
#include <engine/gl/framebuffer.hpp>
#include <engine/gl/shader-program.hpp>
#include <engine/gl/shader-variable.hpp>
#include <engine/gl/vertex-buffer.hpp>
#include <engine/gl/vertex-array.hpp>
#include <engine/gl/vertex-attribute.hpp>
#include <engine/gl/drawing-mode.hpp>
#include <engine/gl/texture-2d.hpp>
#include <engine/gl/texture-wrapping.hpp>
#include <engine/gl/texture-filter.hpp>
#include <engine/render/vertex-attribute.hpp>
#include <engine/render/context.hpp>
#include <cmath>
#include <glad/glad.h>
#include <engine/utility/hash/fnv1a.hpp>

namespace render {

final_pass::final_pass(gl::rasterizer* rasterizer, const gl::framebuffer* framebuffer, resource_manager* resource_manager):
	pass(rasterizer, framebuffer),
	color_texture(nullptr),
	bloom_texture(nullptr),
	bloom_weight(0.04f),
	blue_noise_texture(nullptr),
	blue_noise_scale(1.0f)
{
	// Load shader template and build shader program
	auto shader_template = resource_manager->load<gl::shader_template>("final.glsl");
	shader_program = shader_template->build();
	
	const float2 vertex_positions[] =
	{
		{-1.0f,  1.0f},
		{-1.0f, -1.0f},
		{ 1.0f,  1.0f},
		{ 1.0f,  1.0f},
		{-1.0f, -1.0f},
		{ 1.0f, -1.0f}
	};
	
	const auto vertex_data = std::as_bytes(std::span{vertex_positions});
	std::size_t vertex_size = 2;
	std::size_t vertex_stride = sizeof(float) * vertex_size;
	
	quad_vbo = std::make_unique<gl::vertex_buffer>(gl::buffer_usage::static_draw, vertex_data.size(), vertex_data);
	quad_vao = std::make_unique<gl::vertex_array>();
	
	// Define position vertex attribute
	gl::vertex_attribute position_attribute;
	position_attribute.buffer = quad_vbo.get();
	position_attribute.offset = 0;
	position_attribute.stride = vertex_stride;
	position_attribute.type = gl::vertex_attribute_type::float_32;
	position_attribute.components = 2;
	
	// Bind vertex attributes to VAO
	quad_vao->bind(render::vertex_attribute::position, position_attribute);
}

void final_pass::render(const render::context& ctx, render::queue& queue)
{
	// Update resolution
	const auto viewport_size = framebuffer->get_dimensions();
	resolution = {static_cast<float>(std::get<0>(viewport_size)), static_cast<float>(std::get<1>(viewport_size))};
	
	// Update time
	time = ctx.t;
	
	// Execute render commands
	for (const auto& command: command_buffer)
	{
		command();
	}
}

void final_pass::set_color_texture(const gl::texture_2d* texture)
{
	this->color_texture = texture;
	
	rebuild_command_buffer();
}

void final_pass::set_bloom_texture(const gl::texture_2d* texture) noexcept
{
	this->bloom_texture = texture;
	
	rebuild_command_buffer();
}

void final_pass::set_bloom_weight(float weight) noexcept
{
	this->bloom_weight = weight;
}

void final_pass::set_blue_noise_texture(std::shared_ptr<gl::texture_2d> texture)
{
	this->blue_noise_texture = texture;
	blue_noise_scale = 1.0f / static_cast<float>(texture->get_dimensions()[0]);
	
	rebuild_command_buffer();
}

void final_pass::rebuild_command_buffer()
{
	command_buffer.clear();
	
	command_buffer.emplace_back
	(
		[&]()
		{
			rasterizer->use_framebuffer(*framebuffer);
			rasterizer->set_viewport(0, 0, static_cast<int>(resolution.x()), static_cast<int>(resolution.y()));
			
			glDisable(GL_BLEND);
			glDisable(GL_DEPTH_TEST);
			glDepthMask(GL_FALSE);
			glEnable(GL_CULL_FACE);
			glCullFace(GL_BACK);
			
			rasterizer->use_program(*shader_program);
		}
	);
	
	if (color_texture)
	{
		if (const auto var = shader_program->variable("color_texture"))
		{
			command_buffer.emplace_back([&, var](){var->update(*color_texture);});
		}
	}
	if (bloom_texture)
	{
		if (const auto var = shader_program->variable("bloom_texture"))
		{
			command_buffer.emplace_back([&, var](){var->update(*bloom_texture);});
		}
	}
	if (blue_noise_texture)
	{
		if (const auto var = shader_program->variable("blue_noise_texture"))
		{
			command_buffer.emplace_back([&, var](){var->update(*blue_noise_texture);});
		}
	}
	
	if (const auto var = shader_program->variable("bloom_weight"))
	{
		command_buffer.emplace_back([&, var](){var->update(bloom_weight);});
	}
	if (const auto var = shader_program->variable("blue_noise_scale"))
	{
		command_buffer.emplace_back([&, var](){var->update(blue_noise_scale);});
	}
	if (const auto var = shader_program->variable("resolution"))
	{
		command_buffer.emplace_back([&, var](){var->update(resolution);});
	}
	if (const auto var = shader_program->variable("time"))
	{
		command_buffer.emplace_back([&, var](){var->update(time);});
	}
	
	command_buffer.emplace_back
	(
		[&]()
		{
			rasterizer->draw_arrays(*quad_vao, gl::drawing_mode::triangles, 0, 6);
		}
	);
}

} // namespace render