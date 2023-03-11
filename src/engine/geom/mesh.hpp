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

#ifndef ANTKEEPER_GEOM_MESH_HPP
#define ANTKEEPER_GEOM_MESH_HPP

#include <engine/utility/fundamental-types.hpp>
#include <vector>

namespace geom {

/**
 * Half-edge mesh.
 *
 * @see http://kaba.hilvi.org/homepage/blog/halfedge/halfedge.htm
 */
class mesh
{
public:
	struct vertex;
	struct edge;
	struct face;
	
	/**
	 * Half-edge mesh vertex, containing its index, a pointer to its parent edge, and its position vector.
	 */
	struct vertex
	{
		/// Index of this vertex.
		std::size_t index;
		
		/// Pointer to the edge to which this vertex belongs.
		mesh::edge* edge;
		
		/// Vertex position.
		float3 position;
	};
	
	/**
	 * Half-edge mesh edge, containing its index and pointers to its starting vertex, parent face, and related edges.
	 */
	struct edge
	{
		/// Index of this edge.
		std::size_t index;
		
		/// Pointer to the vertex at which the edge starts.
		mesh::vertex* vertex;     
		
		/// Pointer to the face on the left of this edge.
		mesh::face* face;
		
		/// Pointer to the previous edge in the parent face.
		mesh::edge* previous;
		
		/// Pointer to the next edge in the parent face.
		mesh::edge* next;
		
		/// Pointer to the symmetric edge.
		mesh::edge* symmetric;
	};
	
	/**
	 * Half-edge mesh face, containing its index and a pointer to its first edge.
	 */
	struct face
	{
		/// Index of this face.
		std::size_t index;
		
		/// Pointer to the first edge in this face.
		mesh::edge* edge;
	};
	
	/**
	 * List of edges which form a face.
	 */
	typedef std::vector<edge*> loop;
	
	/**
	 * Constructs a mesh.
	 */
	constexpr mesh() noexcept = default;
	
	/**
	 * Copy-constructs a mesh.
	 */
	mesh(const mesh& other);
	
	/**
	 * Destructs a mesh.
	 */
	~mesh() noexcept;
	
	/**
	 * Copies another mesh into this mesh.
	 *
	 * @param other Mesh to copy.
	 *
	 * @return Reference to this mesh.
	 */
	mesh& operator=(const mesh& other);
	
	/**
	 * Removes all vertices, edges, and faces from the mesh.
	 */
	void clear() noexcept;
	
	/**
	 * Adds a vertex to the mesh.
	 *
	 * @param position Position of the vertex.
	 *
	 * @return Pointer to the added vertex.
	 *
	 * @warning The added vertex will initially have a null edge.
	 */
	mesh::vertex* add_vertex(const float3& position);
	
	/**
	 * Adds two half edges to the mesh.
	 *
	 * @param a Vertex from which the edge originates.
	 * @param b Vertex at which the edge ends.
	 *
	 * @return Pointer to the half edge `ab`. The symmetric half edge `ba` can be accessed through `ab->symmetric`.
	 */
	mesh::edge* add_edge(mesh::vertex* a, mesh::vertex* b);
	
	/**
	 * Adds a face to the mesh.
	 *
	 * @param loop List of edges which form the face.
	 * @return Pointer to the added face.
	 *
	 * @exception std::runtime_error Empty edge loop.
	 * @exception std::runtime_error Disconnected edge loop.
	 * @exception std::runtime_error Non-manifold mesh 1.
	 * @exception std::runtime_error Non-manifold mesh 2.
	 */
	mesh::face* add_face(const loop& loop);
	
	/**
	 * Removes a face from the mesh.
	 *
	 * @param face Face to be removed. This face will be deallocated after removal.
	 */
	void remove_face(mesh::face* face);
	
	/**
	 * Removes an edge and all dependent faces from the mesh.
	 *
	 * @param edge Edge to be removed. This edge will be deallocated after removal.
	 */
	void remove_edge(mesh::edge* edge);
	
	/**
	 * Removes a vertex, all dependent edges, and all dependent faces from the mesh.
	 *
	 * @param vertex Vertex to be removed. This vertex will be deallocated after removal.
	 */
	void remove_vertex(mesh::vertex* vertex);
	
	/// Returns the mesh vertices.
	const std::vector<mesh::vertex*>& get_vertices() const;
	
	/// Returns the mesh edges.
	const std::vector<mesh::edge*>& get_edges() const;
	
	/// Returns the mesh faces.
	const std::vector<mesh::face*>& get_faces() const;
	
private:
	mesh::edge* find_free_incident(mesh::vertex* vertex) const;
	mesh::edge* find_free_incident(mesh::edge* start_edge, mesh::edge* end_edge) const;
	bool make_adjacent(mesh::edge* in_edge, mesh::edge* out_edge);
	
	std::vector<mesh::vertex*> vertices;
	std::vector<mesh::edge*> edges;
	std::vector<mesh::face*> faces;
};

inline const std::vector<mesh::vertex*>& mesh::get_vertices() const
{
	return vertices;
}

inline const std::vector<mesh::edge*>& mesh::get_edges() const
{
	return edges;
}

inline const std::vector<mesh::face*>& mesh::get_faces() const
{
	return faces;
}

} // namespace geom

#endif // ANTKEEPER_GEOM_MESH_HPP