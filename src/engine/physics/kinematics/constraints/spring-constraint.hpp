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

#ifndef ANTKEEPER_PHYSICS_SPRING_CONSTRAINT_HPP
#define ANTKEEPER_PHYSICS_SPRING_CONSTRAINT_HPP

#include <engine/physics/kinematics/constraint.hpp>
#include <engine/physics/kinematics/rigid-body.hpp>
#include <engine/math/vector.hpp>

namespace physics {

/**
 * Spring constraint.
 */
class spring_constraint: public constraint
{
public:
	void solve(float dt) override;
	
	/**
	 * Attaches the spring to body a.
	 *
	 * @param body_a Body to which the spring should be attached.
	 * @param point_a Point on body a, in body-space, at which the spring should be attached.
	 */
	inline constexpr void attach_a(rigid_body& body_a, const math::fvec3& point_a) noexcept
	{
		m_body_a = &body_a;
		m_point_a = point_a;
	}
	
	/**
	 * Attaches the spring to body b.
	 *
	 * @param body_b Body to which the spring should be attached.
	 * @param point_b Point on body b, in body-space, at which the spring should be attached.
	 */
	inline constexpr void attach_b(rigid_body& body_b, const math::fvec3& point_b) noexcept
	{
		m_body_b = &body_b;
		m_point_b = point_b;
	}
	
	/**
	 * Detaches the spring from body a.
	 */
	inline constexpr void detach_a() noexcept
	{
		m_body_a = nullptr;
	}
	
	/**
	 * Detaches the spring from body b.
	 */
	inline constexpr void detach_b() noexcept
	{
		m_body_b = nullptr;
	}
	
	/**
	 * Detaches the spring from bodies a and b.
	 */
	inline constexpr void detach() noexcept
	{
		detach_a();
		detach_b();
	}
	
	/**
	 * Sets the resting length of the spring.
	 *
	 * @param length Resting length, in meters.
	 */
	inline constexpr void set_resting_length(float length) noexcept
	{
		m_resting_length = length;
	}
	
	/**
	 * Sets the stiffness constant of the spring.
	 *
	 * @param stiffness Stiffness constant.
	 */
	inline constexpr void set_stiffness(float stiffness) noexcept
	{
		m_stiffness = stiffness;
	}
	
	/**
	 * Sets the damping constant of the spring.
	 *
	 * @param damping Damping constant.
	 */
	inline constexpr void set_damping(float damping) noexcept
	{
		m_damping = damping;
	}
	
	/// Returns the body to which the spring is attached at point a.
	[[nodiscard]] inline constexpr rigid_body* get_body_a() const noexcept
	{
		return m_body_a;
	}
	
	/// Returns the body to which the spring is attached at point b.
	[[nodiscard]] inline constexpr rigid_body* get_body_b() const noexcept
	{
		return m_body_b;
	}
	
	/// Returns the point at which the spring is attached to body a, in body-space.
	[[nodiscard]] inline constexpr const math::fvec3& get_point_a() const noexcept
	{
		return m_point_a;
	}
	
	/// Returns the point at which the spring is attached to body b, in body-space.
	[[nodiscard]] inline constexpr const math::fvec3& get_point_b() const noexcept
	{
		return m_point_b;
	}
	
	/// Returns the resting length of the spring, in meters.
	[[nodiscard]] inline constexpr float get_resting_length() const noexcept
	{
		return m_resting_length;
	}
	
	/// Returns the stiffness constant of the spring.
	[[nodiscard]] inline constexpr float get_stiffness() const noexcept
	{
		return m_stiffness;
	}
	
	/// Returns the damping constant of the spring.
	[[nodiscard]] inline constexpr float get_damping() const noexcept
	{
		return m_damping;
	}
	
private:
	/// Rigid body to which the spring is attached at point a.
	rigid_body* m_body_a{};
	
	/// Rigid body to which the spring is attached at point b.
	rigid_body* m_body_b{};
	
	/// Point at which the spring is attached to body a, in body-space.
	math::fvec3 m_point_a{};
	
	/// Point at which the spring is attached to body b, in body-space.
	math::fvec3 m_point_b{};
	
	/// Resting length of the spring, in meters.
	float m_resting_length{};
	
	/// Stiffness constant of the spring.
	float m_stiffness{1.0f};
	
	/// Damping constant of the spring.
	float m_damping{1.0f};
};

} // namespace physics

#endif // ANTKEEPER_PHYSICS_SPRING_CONSTRAINT_HPP
