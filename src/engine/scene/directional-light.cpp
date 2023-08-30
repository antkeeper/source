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

#include <engine/scene/directional-light.hpp>

namespace scene {

directional_light::directional_light():
	m_shadow_cascade_distances(m_shadow_cascade_count),
	m_shadow_cascade_matrices(m_shadow_cascade_count),
	m_shadow_scale_bias_matrices(m_shadow_cascade_count)
{
	set_shadow_bias(m_shadow_bias);
}

void directional_light::set_direction(const math::fvec3& direction)
{
	set_rotation(math::rotation(math::fvec3{0.0f, 0.0f, -1.0f}, direction));
}

void directional_light::set_shadow_caster(bool caster) noexcept
{
	m_shadow_caster = caster;
}

void directional_light::set_shadow_framebuffer(std::shared_ptr<gl::framebuffer> framebuffer) noexcept
{
	m_shadow_framebuffer = std::move(framebuffer);
}

void directional_light::set_shadow_bias(float bias) noexcept
{
	m_shadow_bias = bias;
	update_shadow_scale_bias_matrices();
}

void directional_light::set_shadow_cascade_count(unsigned int count) noexcept
{
	m_shadow_cascade_count = count;
	m_shadow_cascade_distances.resize(m_shadow_cascade_count);
	m_shadow_cascade_matrices.resize(m_shadow_cascade_count);
	m_shadow_scale_bias_matrices.resize(m_shadow_cascade_count);
	update_shadow_scale_bias_matrices();
}

void directional_light::set_shadow_cascade_coverage(float factor) noexcept
{
	m_shadow_cascade_coverage = factor;
}

void directional_light::set_shadow_cascade_distribution(float weight) noexcept
{
	m_shadow_cascade_distribution = weight;
}

void directional_light::transformed()
{
	m_direction = get_rotation() * math::fvec3{0.0f, 0.0f, -1.0f};
}

void directional_light::color_updated()
{
	m_colored_illuminance = m_color * m_illuminance;
}

void directional_light::illuminance_updated()
{
	m_colored_illuminance = m_color * m_illuminance;
}

void directional_light::update_shadow_scale_bias_matrices()
{
	// Construct shadow scale-bias matrix (depth range `[-1, 1]`)
	// auto m = math::translate(math::fvec3{0.5f, 0.5f, 0.5f + m_shadow_bias}) * math::scale(math::fvec3{0.5f, 0.5f, 0.5f});
	
	// Construct shadow scale-bias matrix (depth range `[0, 1]`)
	auto m = math::translate(math::fvec3{0.5f, 0.5f, m_shadow_bias}) * math::scale(math::fvec3{0.5f, 0.5f, 1.0f});
	
	// Apply cascade scale
	m = math::scale(math::fvec3{0.5f, 0.5f, 1.0f}) * m;
	
	for (unsigned int i = 0; i < m_shadow_cascade_count; ++i)
	{
		// Apply cascade bias
		m_shadow_scale_bias_matrices[i] = math::translate(math::fvec3{static_cast<float>(i % 2) * 0.5f, static_cast<float>(i / 2) * 0.5f, 0.0f}) * m;
	}
}

} // namespace scene
