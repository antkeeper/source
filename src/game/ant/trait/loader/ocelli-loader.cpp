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

#include "resources/resource-loader.hpp"
#include "resources/resource-manager.hpp"
#include "resources/json.hpp"
#include "game/ant/trait/ocelli.hpp"
#include "render/model.hpp"
#include <stdexcept>

using namespace game::ant;

template <>
trait::ocelli* resource_loader<trait::ocelli>::load(resource_manager* resource_manager, PHYSFS_File* file, const std::filesystem::path& path)
{
	// Load JSON data
	json* data = resource_loader<json>::load(resource_manager, file, path);
	
	// Validate trait file
	auto ocelli_element = data->find("ocelli");
	if (ocelli_element == data->end())
		throw std::runtime_error("Invalid ocelli trait.");
	
	// Allocate ocelli trait
	trait::ocelli* ocelli = new trait::ocelli();
	
	// Parse lateral ocelli
	ocelli->lateral_ocelli = false;
	if (auto lateral_ocelli_element = ocelli_element->find("lateral_ocelli"); lateral_ocelli_element != ocelli_element->end())
		ocelli->lateral_ocelli = lateral_ocelli_element->get<bool>();
	
	// Parse median ocellus
	ocelli->median_ocellus = false;
	if (auto median_ocellus_element = ocelli_element->find("median_ocellus"); median_ocellus_element != ocelli_element->end())
		ocelli->median_ocellus = median_ocellus_element->get<bool>();
	
	// Parse ocelli width
	ocelli->width = 0.0f;
	if (auto width_element = ocelli_element->find("width"); width_element != ocelli_element->end())
		ocelli->width = width_element->get<float>();
	
	// Parse ocelli height
	ocelli->height = 0.0f;
	if (auto height_element = ocelli_element->find("height"); height_element != ocelli_element->end())
		ocelli->height = height_element->get<float>();
	
	// Load lateral ocelli model (if not null)
	if (ocelli->lateral_ocelli)
	{
		auto lateral_ocelli_model_element = ocelli_element->find("lateral_ocelli_model");
		if (lateral_ocelli_model_element == ocelli_element->end() || lateral_ocelli_model_element->is_null())
			throw std::runtime_error("Ocelli trait doesn't specify lateral ocelli model.");
		
		ocelli->lateral_ocelli_model = resource_manager->load<render::model>(lateral_ocelli_model_element->get<std::string>());
	}
	else
	{
		ocelli->lateral_ocelli_model = nullptr;
	}
	
	// Load median ocellus model (if not null)
	if (ocelli->median_ocellus)
	{
		auto median_ocellus_model_element = ocelli_element->find("median_ocellus_model");
		if (median_ocellus_model_element == ocelli_element->end() || median_ocellus_model_element->is_null())
			throw std::runtime_error("Ocelli trait doesn't specify median ocellus model.");
		
		ocelli->median_ocellus_model = resource_manager->load<render::model>(median_ocellus_model_element->get<std::string>());
	}
	else
	{
		ocelli->median_ocellus_model = nullptr;
	}
	
	// Free JSON data
	delete data;
	
	return ocelli;
}