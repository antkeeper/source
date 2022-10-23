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

#include "game/system/terrain.hpp"
#include "game/component/terrain.hpp"
#include "game/component/camera.hpp"
#include "geom/meshes/grid.hpp"
#include "geom/mesh-functions.hpp"
#include "geom/morton.hpp"
#include "geom/quadtree.hpp"
#include "geom/primitive/ray.hpp"
#include "gl/vertex-attribute.hpp"
#include "math/quaternion.hpp"
#include "render/vertex-attribute.hpp"
#include "utility/fundamental-types.hpp"
#include <functional>
#include <iostream>

namespace game {
namespace system {

terrain::terrain(entity::registry& registry):
	updatable(registry),
	patch_side_length(0.0f),
	patch_subdivisions(0),
	patch_material(nullptr),
	elevation_function(nullptr),
	scene_collection(nullptr),
	patch_base_mesh(nullptr),
	patch_vertex_size(0),
	patch_vertex_stride(0),
	patch_vertex_data(nullptr)
{
	// Specify vertex size and stride
	// (position + uv + normal + tangent + barycentric + target)
	patch_vertex_size = 3 + 2 + 3 + 4 + 3 + 3;
	patch_vertex_stride = patch_vertex_size * sizeof(float);
	
	// Init quadtee node sizes at each depth
	for (std::size_t i = 0; i <= quadtree_type::max_depth; ++i)
		quadtree_node_size[i] = 0.0f;
	
	std::cout << "quadtree cap: " << quadtree.max_size() << std::endl;
	
	registry.on_construct<component::terrain>().connect<&terrain::on_terrain_construct>(this);
	registry.on_update<component::terrain>().connect<&terrain::on_terrain_update>(this);
	registry.on_destroy<component::terrain>().connect<&terrain::on_terrain_destroy>(this);
}

terrain::~terrain()
{
	registry.on_construct<component::terrain>().disconnect<&terrain::on_terrain_construct>(this);
	registry.on_update<component::terrain>().disconnect<&terrain::on_terrain_update>(this);
	registry.on_destroy<component::terrain>().disconnect<&terrain::on_terrain_destroy>(this);
}

void terrain::update(double t, double dt)
{
	// Clear quadtree
	quadtree.clear();
	
	// For each camera
	this->registry.view<component::camera>().each
	(
		[&](entity::id camera_eid, const auto& camera)
		{
			if (!camera.object)
				return;
			
			const scene::camera& cam = *camera.object;
			

			

			
			// for (int i = 0; i < 8; ++i)
				// std::cout << "corner " << i << ": " << cam.get_view_frustum().get_corners()[i] << std::endl;
			
			geom::primitive::ray<float, 3> rays[8];
			rays[0] = cam.pick({-1, -1});
			rays[1] = cam.pick({-1,  1});
			rays[2] = cam.pick({ 1,  1});
			rays[3] = cam.pick({ 1, -1});
			
			float3 ntl = rays[0].origin;
			float3 nbl = rays[1].origin;
			float3 nbr = rays[2].origin;
			float3 ntr = rays[3].origin;
			
			float3 ftl = rays[0].origin + rays[0].direction * (cam.get_clip_far() - cam.get_clip_near());
			float3 fbl = rays[1].origin + rays[1].direction * (cam.get_clip_far() - cam.get_clip_near());
			float3 fbr = rays[2].origin + rays[2].direction * (cam.get_clip_far() - cam.get_clip_near());
			float3 ftr = rays[3].origin + rays[3].direction * (cam.get_clip_far() - cam.get_clip_near());
			
			// for (int i = 0; i < 8; ++i)
				// std::cout << "ray or " << i << ": " << rays[i].origin << std::endl;
			
			geom::convex_hull<float> hull(6);
			hull.planes[0] = geom::plane<float>(ftl, fbl, nbl);
			hull.planes[1] = geom::plane<float>(ntr, nbr, fbr);
			hull.planes[2] = geom::plane<float>(fbl, fbr, nbr);
			hull.planes[3] = geom::plane<float>(ftl, ntl, ntr);
			hull.planes[4] = geom::plane<float>(ntl, nbl, nbr);
			hull.planes[5] = geom::plane<float>(ftr, fbr, fbl);
			
			geom::sphere<float> sphere;
			sphere.center = cam.get_translation();
			sphere.radius = patch_side_length;
			
			//visit_quadtree(cam.get_view_frustum().get_bounds(), quadtree_type::root);
			visit_quadtree(sphere, quadtree_type::root);
		}
	);
	
	//std::cout << "qsize: " << quadtree.size() << std::endl;
	std::size_t qvis = 0;

	
	/// Toggle visibility of terrain scene objects
	for (auto it = patches.begin(); it != patches.end(); ++it)
	{
		bool active = (quadtree.contains(it->first) && quadtree.is_leaf(it->first));
		it->second->model_instance->set_active(active);
		
		if (active)
			++qvis;
	}
	
	//std::cout << "qvis: " << qvis << std::endl;
}

void terrain::set_patch_side_length(float length)
{
	patch_side_length = length;
	
	// Recalculate node sizes at each quadtree depth
	for (std::size_t i = 0; i <= quadtree_type::max_depth; ++i)
	{
		quadtree_node_size[i] = std::exp2(quadtree_type::max_depth - i) * patch_side_length;
		//std::cout << quadtree_node_size[i] << std::endl;
	}
}

void terrain::set_patch_subdivisions(std::size_t n)
{
	patch_subdivisions = n;
	
	// Recalculate patch properties
	patch_cell_count = (patch_subdivisions + 1) * (patch_subdivisions + 1);
	patch_triangle_count = patch_cell_count * 2;
	
	// Resize patch vertex data buffer
	delete[] patch_vertex_data;
	patch_vertex_data = new float[patch_triangle_count * 3 * patch_vertex_size];
	
	// Resize patch buffers
	
	std::size_t vertex_buffer_row_size = patch_subdivisions + 4;
	std::size_t vertex_buffer_column_size = vertex_buffer_row_size;
	
	patch_vertex_buffer.resize(vertex_buffer_row_size);
	for (std::size_t i = 0; i < patch_vertex_buffer.size(); ++i)
		patch_vertex_buffer[i].resize(vertex_buffer_column_size);
	
	rebuild_patch_base_mesh();
}

void terrain::set_patch_material(::render::material* material)
{
	patch_material = material;
}

void terrain::set_elevation_function(const std::function<float(float, float)>& f)
{
	elevation_function = f;
}

void terrain::set_scene_collection(scene::collection* collection)
{
	scene_collection = collection;
}

void terrain::on_terrain_construct(entity::registry& registry, entity::id entity_id)
{
}

void terrain::on_terrain_update(entity::registry& registry, entity::id entity_id)
{
}

void terrain::on_terrain_destroy(entity::registry& registry, entity::id entity_id)
{
}

float terrain::get_patch_size(quadtree_node_type node) const
{
	return quadtree_node_size[quadtree_type::depth(node)];
}

float3 terrain::get_patch_center(quadtree_node_type node) const
{
	const float node_size = get_patch_size(node);
	const float node_offset = quadtree_node_size[0] * -0.5f + node_size * 0.5f;
	
	// Extract node location from Morton location code
	quadtree_type::node_type node_location = quadtree_type::location(node);
	quadtree_type::node_type node_location_x;
	quadtree_type::node_type node_location_y;
	geom::morton::decode(node_location, node_location_x, node_location_y);
	
	return float3
	{
		node_offset + static_cast<float>(node_location_x) * node_size,
		0.0f,
		node_offset + static_cast<float>(node_location_y) * node_size
	};
}

void terrain::rebuild_patch_base_mesh()
{
	// Rebuild grid
	delete patch_base_mesh;
	patch_base_mesh = geom::meshes::grid_xy(1.0f, patch_subdivisions, patch_subdivisions);
	
	// Convert quads to triangle fans
	for (std::size_t i = 0; i < patch_base_mesh->get_faces().size(); ++i)
	{
		geom::mesh::face* face = patch_base_mesh->get_faces()[i];
		
		std::size_t edge_count = 1;
		for (geom::mesh::edge* edge = face->edge->next; edge != face->edge; edge = edge->next)
			++edge_count;
		
		if (edge_count > 3)
		{
			geom::poke_face(*patch_base_mesh, face->index);
			--i;
		}
	}
	
	// Transform patch base mesh coordinates from XY plane to XZ plane
	const math::quaternion<float> xy_to_xz = math::quaternion<float>::rotate_x(math::half_pi<float>);
	for (geom::mesh::vertex* vertex: patch_base_mesh->get_vertices())
	{
		vertex->position = xy_to_xz * vertex->position;
	}
}

void terrain::visit_quadtree(const geom::bounding_volume<float>& volume, quadtree_node_type node)
{
	const float root_offset = quadtree_node_size[0] * -0.5f;
	
	// Extract node depth
	quadtree_type::node_type node_depth = quadtree_type::depth(node);
	
	const float node_size = get_patch_size(node);
	const float3 node_center = get_patch_center(node);
	
	// Build node bounds AABB
	geom::aabb<float> node_bounds;
	node_bounds.min_point = 
	{
		node_center.x() - node_size * 0.5f,
		-std::numeric_limits<float>::infinity(),
		node_center.z() - node_size * 0.5f
	};
	node_bounds.max_point =
	{
		node_bounds.min_point.x() + node_size,
		std::numeric_limits<float>::infinity(),
		node_bounds.min_point.z() + node_size
	};
	
	// If volume intersects node
	if (volume.intersects(node_bounds))
	{
		// Subdivide leaf nodes
		if (quadtree.is_leaf(node))
		{
			quadtree.insert(quadtree_type::child(node, 0));
			
			for (quadtree_node_type i = 0; i < quadtree_type::children_per_node; ++i)
			{
				quadtree_node_type child = quadtree_type::child(node, i);
				
				if (patches.find(child) == patches.end())
				{
					patch* child_patch = generate_patch(child);
					patches[child] = child_patch;
					scene_collection->add_object(child_patch->model_instance);
				}
			}
		}
		
		// Visit children
		if (node_depth < quadtree_type::max_depth - 1)
		{
			for (quadtree_node_type i = 0; i < quadtree_type::children_per_node; ++i)
				visit_quadtree(volume, quadtree_type::child(node, i));
		}
	}
}

geom::mesh* terrain::generate_patch_mesh(quadtree_node_type node) const
{
	// Extract node depth
	const quadtree_type::node_type node_depth = quadtree_type::depth(node);
	
	// Get size of node at depth
	const float node_size = quadtree_node_size[node_depth];
	
	// Extract node Morton location code and decode location
	const quadtree_type::node_type node_location = quadtree_type::location(node);
	quadtree_type::node_type node_location_x;
	quadtree_type::node_type node_location_y;
	geom::morton::decode(node_location, node_location_x, node_location_y);
	
	// Determine center of node
	const float node_offset = quadtree_node_size[0] * -0.5f + node_size * 0.5f;
	const float3 node_center =
	{
		node_offset + static_cast<float>(node_location_x) * node_size,
		0.0f,
		node_offset + static_cast<float>(node_location_y) * node_size
	};
	
	// Copy patch base mesh
	geom::mesh* patch_mesh = new geom::mesh(*patch_base_mesh);
	
	// Modify patch mesh vertex positions
	for (geom::mesh::vertex* v: patch_mesh->get_vertices())
	{
		v->position.x() = node_center.x() + v->position.x() * node_size;
		v->position.z() = node_center.z() + v->position.z() * node_size;
		v->position.y() = elevation_function(v->position.x(), v->position.z());
	}
	
	return patch_mesh;
}

::render::model* terrain::generate_patch_model(quadtree_node_type node) const
{
	// Get size and position of patch
	const float patch_size = get_patch_size(node);
	const float3 patch_center = get_patch_center(node);
	
	// Calculate size of a patch cell
	const float cell_size = patch_size / static_cast<float>(patch_subdivisions + 1);
	const float half_cell_size = cell_size * 0.5f;
	
	// Init patch bounds
	geom::aabb<float> patch_bounds;
	patch_bounds.min_point.x() = patch_center.x() - patch_size * 0.5f;
	patch_bounds.min_point.y() = std::numeric_limits<float>::infinity();
	patch_bounds.min_point.z() = patch_center.z() - patch_size * 0.5f;
	patch_bounds.max_point.x() = patch_center.x() + patch_size * 0.5f;
	patch_bounds.max_point.y() = -std::numeric_limits<float>::infinity();
	patch_bounds.max_point.z() = patch_center.z() + patch_size * 0.5f;
	
	// Calculate positions and UVs of patch vertices and immediately neighboring vertices
	float3 first_vertex_position =
	{
		patch_bounds.min_point.x() - cell_size,
		patch_center.y(),
		patch_bounds.min_point.z() - cell_size
	};
	float3 vertex_position = first_vertex_position;
	for (std::size_t i = 0; i < patch_vertex_buffer.size(); ++i)
	{
		// For each column
		for (std::size_t j = 0; j < patch_vertex_buffer[i].size(); ++j)
		{
			// Calculate vertex elevation
			vertex_position.y() = elevation_function(vertex_position.x(), vertex_position.z());
			
			// Update patch bounds
			patch_bounds.min_point.y() = std::min(patch_bounds.min_point.y(), vertex_position.y());
			patch_bounds.max_point.y() = std::max(patch_bounds.max_point.y(), vertex_position.y());
			
			// Update patch vertex position
			patch_vertex_buffer[i][j].position = vertex_position;
			
			// Calculate patch vertex UV
			patch_vertex_buffer[i][j].uv.x() = (vertex_position.x() - patch_bounds.min_point.x()) / patch_size;
			patch_vertex_buffer[i][j].uv.y() = (vertex_position.z() - patch_bounds.min_point.z()) / patch_size;
			
			// Init patch vertex normal, tangent, and bitangent
			patch_vertex_buffer[i][j].normal = {0, 0, 0};
			patch_vertex_buffer[i][j].tangent = {0, 0, 0};
			patch_vertex_buffer[i][j].bitangent = {0, 0, 0};
			
			vertex_position.x() += cell_size;
		}
		
		vertex_position.z() += cell_size;
		vertex_position.x() = first_vertex_position.x();
	}
	
	// Accumulate normals, tangents, and bitangents
	for (std::size_t i = 0; i < patch_vertex_buffer.size() - 1; ++i)
	{
		for (std::size_t j = 0; j < patch_vertex_buffer[i].size() - 1; ++j)
		{
			patch_vertex& a = patch_vertex_buffer[i  ][j];
			patch_vertex& b = patch_vertex_buffer[i+1][j];
			patch_vertex& c = patch_vertex_buffer[i  ][j+1];
			patch_vertex& d = patch_vertex_buffer[i+1][j+1];
			
			auto add_ntb = [](auto& a, auto& b, auto& c)
			{
				const float3 ba = b.position - a.position;
				const float3 ca = c.position - a.position;
				const float2 uvba = b.uv - a.uv;
				const float2 uvca = c.uv - a.uv;
				
				const float3 normal = math::normalize(math::cross(ba, ca));
				const float f = 1.0f / (uvba.x() * uvca.y() - uvca.x() * uvba.y());
				const float3 tangent = (ba * uvca.y() - ca * uvba.y()) * f;
				const float3 bitangent = (ba * -uvca.x() + ca * uvba.x()) * f;
				
				a.normal += normal;
				a.tangent += tangent;
				a.bitangent += bitangent;
				
				b.normal += normal;
				b.tangent += tangent;
				b.bitangent += bitangent;
				
				c.normal += normal;
				c.tangent += tangent;
				c.bitangent += bitangent;
			};
			
			if ((j + i) % 2)
			{
				add_ntb(a, b, c);
				add_ntb(c, b, d);
			}
			else
			{
				add_ntb(a, b, d);
				add_ntb(a, d, c);
			}
		}
	}
	
	// Finalize normals, tangents, and bitangent signs of patch vertices
	for (std::size_t i = 1; i < patch_vertex_buffer.size() - 1; ++i)
	{
		for (std::size_t j = 1; j < patch_vertex_buffer[i].size() - 1; ++j)
		{
			auto& vertex = patch_vertex_buffer[i][j];
			
			// Normalize normal
			vertex.normal = math::normalize(vertex.normal);
			
			// Gram-Schmidt orthogonalize tangent
			vertex.tangent = math::normalize(vertex.tangent - vertex.normal * math::dot(vertex.normal, vertex.tangent));
			
			// Calculate bitangent sign
			vertex.bitangent_sign = std::copysign(1.0f, math::dot(math::cross(vertex.normal, vertex.tangent), vertex.bitangent));
		}
	}
	
	/*
	
	0 subdivisions:
	+---+---+---+
	|           |
	+   +---+   +
	|   |   |   |
	+   +---+   +
	|           |
    +---+---+---+
	
	1 subdivision:
	+---+---+---+---+
    |               |
	+   +---+---+   +
	|   |   |   |   |
	+   +---+---+   +
	|   |   |   |   |
	+   +---+---+   +
	|               |
	+---+---+---+---+
	
	2 subdivisions:
	+---+---+---+---+---+
	|                   |
	+   +---+---+---+   +
	|   |   |   |   |   |
	+   +---+---+---+   +
	|   |   |   |   |   |
	+   +---+---+---+   +
	|   |   |   |   |   |
	+   +---+---+---+   +
	|                   |
	+---+---+---+---+---+
	*/
	
	// For each row
	float* v = patch_vertex_data;
	for (std::size_t i = 1; i < patch_vertex_buffer.size() - 2; ++i)
	{
		// For each column
		for (std::size_t j = 1; j < patch_vertex_buffer[i].size() - 2; ++j)
		{
			// a---c
			// |   |
			// b---d
			const patch_vertex& a = patch_vertex_buffer[i  ][j];
			const patch_vertex& b = patch_vertex_buffer[i+1][j];
			const patch_vertex& c = patch_vertex_buffer[i  ][j+1];
			const patch_vertex& d = patch_vertex_buffer[i+1][j+1];
			
			auto add_triangle = [&v](const patch_vertex& a, const patch_vertex& b, const patch_vertex& c)
			{
				auto add_vertex = [&v](const patch_vertex& vertex, const float3& barycentric)
				{
					// Position
					*(v++) = vertex.position[0];
					*(v++) = vertex.position[1];
					*(v++) = vertex.position[2];
					
					// UV
					*(v++) = vertex.uv[0];
					*(v++) = vertex.uv[1];
					
					// Normal
					*(v++) = vertex.normal[0];
					*(v++) = vertex.normal[1];
					*(v++) = vertex.normal[2];
					
					/// Tangent
					*(v++) = vertex.tangent[0];
					*(v++) = vertex.tangent[1];
					*(v++) = vertex.tangent[2];
					*(v++) = vertex.bitangent_sign;
					
					// Barycentric
					*(v++) = barycentric[0];
					*(v++) = barycentric[1];
					*(v++) = barycentric[2];
					
					// Morph target (LOD transition)
					*(v++) = 0.0f;
					*(v++) = 0.0f;
					*(v++) = 0.0f;
				};
				
				add_vertex(a, float3{1, 0, 0});
				add_vertex(b, float3{0, 1, 0});
				add_vertex(c, float3{0, 0, 1});
			};
			
			if ((j + i) % 2)
			{
				add_triangle(a, b, c);
				add_triangle(c, b, d);
			}
			else
			{
				add_triangle(a, b, d);
				add_triangle(a, d, c);
			}
		}
	}
	
	// Allocate patch model
	::render::model* patch_model = new ::render::model();
	
	// Get model VBO and VAO
	gl::vertex_buffer* vbo = patch_model->get_vertex_buffer();
	gl::vertex_array* vao = patch_model->get_vertex_array();

	// Resize model VBO and upload vertex data
	vbo->resize(patch_triangle_count * 3 * patch_vertex_stride, patch_vertex_data);
	
	std::size_t attribute_offset = 0;
	
	// Define position vertex attribute
	gl::vertex_attribute position_attribute;
	position_attribute.buffer = vbo;
	position_attribute.offset = attribute_offset;
	position_attribute.stride = patch_vertex_stride;
	position_attribute.type = gl::vertex_attribute_type::float_32;
	position_attribute.components = 3;
	attribute_offset += position_attribute.components * sizeof(float);
	
	// Define UV vertex attribute
	gl::vertex_attribute uv_attribute;
	uv_attribute.buffer = vbo;
	uv_attribute.offset = attribute_offset;
	uv_attribute.stride = patch_vertex_stride;
	uv_attribute.type = gl::vertex_attribute_type::float_32;
	uv_attribute.components = 2;
	attribute_offset += uv_attribute.components * sizeof(float);
	
	// Define normal vertex attribute
	gl::vertex_attribute normal_attribute;
	normal_attribute.buffer = vbo;
	normal_attribute.offset = attribute_offset;
	normal_attribute.stride = patch_vertex_stride;
	normal_attribute.type = gl::vertex_attribute_type::float_32;
	normal_attribute.components = 3;
	attribute_offset += normal_attribute.components * sizeof(float);
	
	// Define tangent vertex attribute
	gl::vertex_attribute tangent_attribute;
	tangent_attribute.buffer = vbo;
	tangent_attribute.offset = attribute_offset;
	tangent_attribute.stride = patch_vertex_stride;
	tangent_attribute.type = gl::vertex_attribute_type::float_32;
	tangent_attribute.components = 4;
	attribute_offset += tangent_attribute.components * sizeof(float);
	
	// Define barycentric vertex attribute
	gl::vertex_attribute barycentric_attribute;
	barycentric_attribute.buffer = vbo;
	barycentric_attribute.offset = attribute_offset;
	barycentric_attribute.stride = patch_vertex_stride;
	barycentric_attribute.type = gl::vertex_attribute_type::float_32;
	barycentric_attribute.components = 3;
	attribute_offset += barycentric_attribute.components * sizeof(float);
	
	// Define target vertex attribute
	gl::vertex_attribute target_attribute;
	target_attribute.buffer = vbo;
	target_attribute.offset = attribute_offset;
	target_attribute.stride = patch_vertex_stride;
	target_attribute.type = gl::vertex_attribute_type::float_32;
	target_attribute.components = 3;
	attribute_offset += target_attribute.components * sizeof(float);
	
	// Bind vertex attributes to VAO
	vao->bind(::render::vertex_attribute::position, position_attribute);
	vao->bind(::render::vertex_attribute::uv, uv_attribute);
	vao->bind(::render::vertex_attribute::normal, normal_attribute);
	vao->bind(::render::vertex_attribute::tangent, tangent_attribute);
	vao->bind(::render::vertex_attribute::barycentric, barycentric_attribute);
	vao->bind(::render::vertex_attribute::target, target_attribute);
	
	// Create model group
	::render::model_group* patch_model_group = patch_model->add_group("terrain");
	patch_model_group->set_material(patch_material);
	patch_model_group->set_drawing_mode(gl::drawing_mode::triangles);
	patch_model_group->set_start_index(0);
	patch_model_group->set_index_count(patch_triangle_count * 3);
	
	// Set patch model bounds
	patch_model->set_bounds(patch_bounds);
	
	//std::cout << "depth: " << quadtree_type::depth(node) << "; size: " << (patch_bounds.max_point + patch_bounds.min_point) * 0.5f << std::endl;
	
	return patch_model;
}

terrain::patch* terrain::generate_patch(quadtree_node_type node)
{
	patch* node_patch = new patch();
	node_patch->mesh = nullptr;//generate_patch_mesh(node);
	node_patch->model = generate_patch_model(node);
	node_patch->model_instance = new scene::model_instance(node_patch->model);
	return node_patch;
}

} // namespace system
} // namespace game
