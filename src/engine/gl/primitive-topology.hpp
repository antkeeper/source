// SPDX-FileCopyrightText: 2025 C. J. Howard
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef ANTKEEPER_GL_PRIMITIVE_TOPOLOGY_HPP
#define ANTKEEPER_GL_PRIMITIVE_TOPOLOGY_HPP

#include <cstdint>

namespace gl {

/// Primitive topologies.
enum class primitive_topology: std::uint8_t
{
	/// Separate point primitives.
	point_list,
	
	/// Separate line primitives.
	line_list,
	
	/// Connected line primitives with consecutive lines sharing a vertex.
	line_strip,
	
	/// Separate triangle primitives.
	triangle_list,
	
	/// Connected triangle primitives with consecutive triangles sharing an edge.
	triangle_strip,
	
	/// Connected triangle primitives with all triangles sharing a common vertex.
	triangle_fan,
	
	/// Separate line primitives with adjacency.
	line_list_with_adjacency,
	
	/// Connected line primitives with adjacency, with consecutive primitives sharing three vertices.
	line_strip_with_adjacency,
	
	/// Separate triangle primitives with adjacency.
	triangle_list_with_adjacency,
	
	/// Connected triangle primitives with adjacency, with consecutive triangles sharing an edge.
	triangle_strip_with_adjacency,
	
	/// Separate patch primitives.
	patch_list
};

} // namespace gl

#endif // ANTKEEPER_GL_PRIMITIVE_TOPOLOGY_HPP
