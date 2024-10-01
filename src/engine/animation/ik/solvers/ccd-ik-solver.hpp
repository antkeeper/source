// SPDX-FileCopyrightText: 2024 C. J. Howard
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef ANTKEEPER_ANIMATION_CCD_IK_SOLVER_HPP
#define ANTKEEPER_ANIMATION_CCD_IK_SOLVER_HPP

#include <engine/animation/ik/ik-solver.hpp>
#include <engine/animation/bone.hpp>
#include <engine/math/vector.hpp>
#include <vector>

class ik_rig;

/// Cyclic Coordinate Descent (CCD) IK solver.
class ccd_ik_solver: public ik_solver
{
public:
	/// Constructs a CCD IK solver.
	/// @param ik_rig IK rig with which to associate this IK solver.
	/// @param root_bone_index Index of the first bone in the bone chain.
	/// @param effector_bone_index Index of the last bone in the bone chain.
	ccd_ik_solver(ik_rig& ik_rig, std::size_t root_bone_index, std::size_t effector_bone_index);
	
	/// Destructs a CCD IK solver.
	~ccd_ik_solver() override = default;
	
	/// @name Solving
	/// @{
	
	/// Solves the IK chain using the Cyclic Coordinate Descent algorithm.
	void solve() override;
	
	/// Sets the maximum number of solving iterations.
	/// @param iterations Maximum number of solving iterations.
	inline void set_max_iterations(std::size_t iterations) noexcept
	{
		m_max_iterations = iterations;
	}
	
	/// Returns the maximum number of solving iterations.
	[[nodiscard]] inline std::size_t get_max_iterations() const noexcept
	{
		return m_max_iterations;
	}
	
	/// @}
	
	/// @name Effector
	/// @{
	
	/// Sets the position of the end effector.
	/// @param position Position of the end effector, relative to the tip bone.
	inline void set_effector_position(const math::fvec3& position) noexcept
	{
		m_effector_position = position;
	}
	
	/// Returns the position of the end effector, relative to the tip bone.
	[[nodiscard]] inline const math::fvec3& get_effector_position() const
	{
		return m_effector_position;
	}
	
	/// @}
	
	/// @name Goal
	/// @{
	
	/// Sets the center of the IK goal.
	/// @param center IK goal center, in world-space.
	inline void set_goal_center(const math::fvec3& center) noexcept
	{
		m_goal_center = center;
	}
	
	/// Sets the radius of the IK goal.
	/// @param radius IK goal radius.
	inline void set_goal_radius(float radius) noexcept
	{
		m_sqr_goal_radius = radius * radius;
	}
	
	/// Returns the center of the IK goal, in world-space.
	[[nodiscard]] inline const math::fvec3& get_goal_center() const
	{
		return m_goal_center;
	}
	
	/// @}
	
private:
	ik_rig* m_ik_rig{nullptr};
	std::size_t m_max_iterations{10};
	std::vector<std::size_t> m_bone_indices;
	math::fvec3 m_effector_position{0.0f, 0.0f, 0.0f};
	math::fvec3 m_goal_center{0.0f, 0.0f, 0.0f};
	float m_sqr_goal_radius{1e-5f};
};

#endif // ANTKEEPER_ANIMATION_CCD_IK_SOLVER_HPP
