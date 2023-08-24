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

#ifndef ANTKEEPER_GAME_ANT_LARVA_GENE_HPP
#define ANTKEEPER_GAME_ANT_LARVA_GENE_HPP

#include "game/ant/genes/ant-gene.hpp"
#include <engine/render/model.hpp>
#include <cstdint>
#include <memory>

/**
 * Ant larva phene.
 */
struct ant_larva_phene
{
	/// Duration required for the larva to develop into a pupa, in days.
	float development_period{};
	
	/// Duration required for the larva to spin a cocoon, in days.
	float spinning_period{};
	
	/// Number of larval instars before pupation.
	std::uint8_t instar_count{};
	
	/// Scale of the first larva instar, relative to the final larval instar.
	float first_instar_scale{};
	
	/// 3D model of the larva.
	std::shared_ptr<render::model> model;
};

/// Ant larva gene.
using ant_larva_gene = ant_gene<ant_larva_phene>;

template <>
inline constexpr ant_gene_type ant_larva_gene::type() const noexcept
{
	return ant_gene_type::larva;
}

#endif // ANTKEEPER_GAME_ANT_LARVA_GENE_HPP
