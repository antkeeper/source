// SPDX-FileCopyrightText: 2025 C. J. Howard
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef ANTKEEPER_GL_PIPELINE_INPUT_ASSEMBLY_STATE_HPP
#define ANTKEEPER_GL_PIPELINE_INPUT_ASSEMBLY_STATE_HPP

#include <engine/gl/primitive-topology.hpp>
#include <cstdlib>

namespace gl {

/// Pipeline input assembly state.
struct pipeline_input_assembly_state
{
	/// Primitive topology.
	primitive_topology topology{primitive_topology::triangle_list};
	
	/// Controls whether a special vertex index value is treated as restarting the assembly of primitives.
	bool primitive_restart_enabled{false};
};

} // namespace gl

#endif // ANTKEEPER_GL_PIPELINE_INPUT_ASSEMBLY_STATE_HPP
