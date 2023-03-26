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

#ifndef ANTKEEPER_GAME_ANT_BONE_SET_HPP
#define ANTKEEPER_GAME_ANT_BONE_SET_HPP

#include <engine/animation/bone.hpp>
#include <optional>

/**
 * Set of bone indices for all possible bones in an ant skeleotn.
 */
struct ant_bone_set
{
	bone_index_type mesosoma;
	bone_index_type procoxa_l;
	bone_index_type profemur_l;
	bone_index_type protibia_l;
	bone_index_type protarsomere1_l;
	bone_index_type procoxa_r;
	bone_index_type profemur_r;
	bone_index_type protibia_r;
	bone_index_type protarsomere1_r;
	bone_index_type mesocoxa_l;
	bone_index_type mesofemur_l;
	bone_index_type mesotibia_l;
	bone_index_type mesotarsomere1_l;
	bone_index_type mesocoxa_r;
	bone_index_type mesofemur_r;
	bone_index_type mesotibia_r;
	bone_index_type mesotarsomere1_r;
	bone_index_type metacoxa_l;
	bone_index_type metafemur_l;
	bone_index_type metatibia_l;
	bone_index_type metatarsomere1_l;
	bone_index_type metacoxa_r;
	bone_index_type metafemur_r;
	bone_index_type metatibia_r;
	bone_index_type metatarsomere1_r;
	bone_index_type head;
	bone_index_type mandible_l;
	bone_index_type mandible_r;
	bone_index_type antennomere1_l;
	bone_index_type antennomere2_l;
	bone_index_type antennomere1_r;
	bone_index_type antennomere2_r;
	std::optional<bone_index_type> petiole;
	std::optional<bone_index_type> postpetiole;
	bone_index_type gaster;
	std::optional<bone_index_type> sting;
	std::optional<bone_index_type> forewing_l;
	std::optional<bone_index_type> forewing_r;
	std::optional<bone_index_type> hindwing_l;
	std::optional<bone_index_type> hindwing_r;
};

#endif // ANTKEEPER_GAME_ANT_BONE_SET_HPP
