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

#ifndef ANTKEEPER_GEOM_OCTREE_HPP
#define ANTKEEPER_GEOM_OCTREE_HPP

#include "geom/hyperoctree.hpp"

namespace geom {

/// An octree, or 3-dimensional hyperoctree.
template <class T, std::size_t D>
using octree = hyperoctree<T, 3, D>;

/// Octree with an 8-bit node type (2 depth levels).
typedef octree<std::uint8_t, 1> octree8;

/// Octree with a 16-bit node type (4 depth levels).
typedef octree<std::uint16_t, 3> octree16;

/// Octree with a 32-bit node type (9 depth levels).
typedef octree<std::uint32_t, 8> octree32;

/// Octree with a 64-bit node type (19 depth levels).
typedef octree<std::uint64_t, 18> octree64;

} // namespace geom

#endif // ANTKEEPER_GEOM_OCTREE_HPP
