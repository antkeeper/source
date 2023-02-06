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

#ifndef ANTKEEPER_RENDER_ANTI_ALIASING_METHOD_HPP
#define ANTKEEPER_RENDER_ANTI_ALIASING_METHOD_HPP

#include <cstdint>

namespace render {

/**
 * Anti-aliasing methods.
 */
enum class anti_aliasing_method: std::uint8_t
{
	/// No anti-aliasing.
	none,
	
	/// Fast approximate anti-aliasing (FXAA).
	fxaa
};

} // namespace render

#endif // ANTKEEPER_RENDER_ANTI_ALIASING_METHOD_HPP