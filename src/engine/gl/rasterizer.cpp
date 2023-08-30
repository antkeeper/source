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

#include <engine/gl/rasterizer.hpp>
#include <engine/gl/framebuffer.hpp>
#include <engine/gl/shader-program.hpp>
#include <engine/gl/vertex-array.hpp>
#include <glad/glad.h>
#include <engine/debug/log.hpp>
#include <stdexcept>

namespace gl {

static constexpr GLenum drawing_mode_lut[] =
{
	GL_POINTS,
	GL_LINE_STRIP,
	GL_LINE_LOOP,
	GL_LINES,
	GL_LINE_STRIP_ADJACENCY,
	GL_LINES_ADJACENCY,
	GL_TRIANGLE_STRIP,
	GL_TRIANGLE_FAN,
	GL_TRIANGLES,
	GL_TRIANGLE_STRIP_ADJACENCY,
	GL_TRIANGLES_ADJACENCY
};

static constexpr GLenum element_array_type_lut[] =
{
	GL_UNSIGNED_BYTE,
	GL_UNSIGNED_SHORT,
	GL_UNSIGNED_INT
};

rasterizer::rasterizer():
	bound_vao(nullptr),
	bound_shader_program(nullptr)
{
	// Determine dimensions of default framebuffer
	GLint scissor_box[4] = {0, 0, 0, 0};
	glGetIntegerv(GL_SCISSOR_BOX, scissor_box);
	
	// Setup default framebuffer
	default_framebuffer = std::make_unique<framebuffer>();
	default_framebuffer->m_gl_framebuffer_id = 0;
	default_framebuffer->m_dimensions = {scissor_box[2], scissor_box[3]};
	
	// Bind default framebuffer
	bound_framebuffer = default_framebuffer.get();
	
	// Enable seamless filtering across cubemap faces
	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
	
	if (GLAD_GL_ARB_clip_control)
	{
		// Improve depth buffer precision by setting depth range to `[0, 1]`.
		glClipControl(GL_LOWER_LEFT, GL_ZERO_TO_ONE);
	}
	else
	{
		debug::log::error("glClipControl not supported");
		throw std::runtime_error("glClipControl not supported");
	}
	
	// glClipControl alternative for OpenGL < v4.5 (need to adjust shadow scale-bias matrix too)
	// glDepthRange(-1.0f, 1.0f);
	
	// Set clear depth to `0` for reversed depth
	glClearDepth(0.0f);
}

rasterizer::~rasterizer()
{}

void rasterizer::context_resized(int width, int height)
{
	default_framebuffer->m_dimensions = {width, height};
}

void rasterizer::use_framebuffer(const gl::framebuffer& framebuffer)
{
	if (bound_framebuffer != &framebuffer)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, framebuffer.m_gl_framebuffer_id);
		bound_framebuffer = &framebuffer;
	}
}

void rasterizer::set_clear_color(float r, float g, float b, float a)
{
	glClearColor(r, g, b, a);
}

void rasterizer::set_clear_depth(float depth)
{
	glClearDepth(depth);
}

void rasterizer::set_clear_stencil(int s)
{
	glClearStencil(s);
}

void rasterizer::clear_framebuffer(bool color, bool depth, bool stencil)
{
	GLbitfield mask = 0;

	if (color)
		mask |= GL_COLOR_BUFFER_BIT;
	if (depth)
		mask |= GL_DEPTH_BUFFER_BIT;
	if (stencil)
		mask |= GL_STENCIL_BUFFER_BIT;

	glClear(mask);
}

void rasterizer::set_viewport(int x, int y, int width, int height)
{
	glViewport(x, y, static_cast<GLsizei>(width), static_cast<GLsizei>(height));
}

void rasterizer::use_program(const shader_program& program)
{
	if (bound_shader_program != &program)
	{
		glUseProgram(program.gl_program_id);
		bound_shader_program = &program;
	}
}

void rasterizer::draw_arrays(const vertex_array& vao, drawing_mode mode, std::size_t offset, std::size_t count)
{
	GLenum gl_mode = drawing_mode_lut[static_cast<std::size_t>(mode)];

	if (bound_vao != &vao)
	{
		glBindVertexArray(vao.gl_array_id);
		bound_vao = &vao;
	}

	glDrawArrays(gl_mode, static_cast<GLint>(offset), static_cast<GLsizei>(count));
}

void rasterizer::draw_arrays_instanced(const vertex_array& vao, drawing_mode mode, std::size_t offset, std::size_t count, std::size_t instance_count)
{
	GLenum gl_mode = drawing_mode_lut[static_cast<std::size_t>(mode)];

	if (bound_vao != &vao)
	{
		glBindVertexArray(vao.gl_array_id);
		bound_vao = &vao;
	}

	glDrawArraysInstanced(gl_mode, static_cast<GLint>(offset), static_cast<GLsizei>(count), static_cast<GLsizei>(instance_count));
}

void rasterizer::draw_elements(const vertex_array& vao, drawing_mode mode, std::size_t offset, std::size_t count, element_array_type type)
{
	GLenum gl_mode = drawing_mode_lut[static_cast<std::size_t>(mode)];
	GLenum gl_type = element_array_type_lut[static_cast<std::size_t>(type)];

	if (bound_vao != &vao)
	{
		glBindVertexArray(vao.gl_array_id);
		bound_vao = &vao;
	}

	glDrawElements(gl_mode, static_cast<GLsizei>(count), gl_type, reinterpret_cast<const GLvoid*>(offset));
}

} // namespace gl
