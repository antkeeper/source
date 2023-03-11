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

#ifndef ANTKEEPER_GAME_ANT_PILOSITY_GENE_HPP
#define ANTKEEPER_GAME_ANT_PILOSITY_GENE_HPP

#include "game/ant/genes/ant-gene.hpp"

/**
 * Ant pilosity phene.
 */
struct ant_pilosity_phene
{
	/// Hair density.
	float density{0.0f};
};

/// Ant pilosity gene.
using ant_pilosity_gene = ant_gene<ant_pilosity_phene>;

template <>
inline constexpr ant_gene_type ant_pilosity_gene::type() const noexcept
{
	return ant_gene_type::pilosity;
}

#endif // ANTKEEPER_GAME_ANT_PILOSITY_GENE_HPP