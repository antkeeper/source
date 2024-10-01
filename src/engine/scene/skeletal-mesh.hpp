// SPDX-FileCopyrightText: 2024 C. J. Howard
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef ANTKEEPER_SCENE_SKELETAL_MESH_HPP
#define ANTKEEPER_SCENE_SKELETAL_MESH_HPP

#include <engine/scene/object.hpp>
#include <engine/render/model.hpp>
#include <engine/render/operation.hpp>
#include <engine/animation/skeleton-pose.hpp>
#include <vector>

namespace scene {

/// Skeletal mesh object.
class skeletal_mesh: public object<skeletal_mesh>
{
public:
	/// Constructs a skeletal mesh from a model.
	/// @param model Model from which the skeletal mesh will be constructed.
	explicit skeletal_mesh(std::shared_ptr<render::model> model);
	
	/// Constructs a skeletal mesh.
	skeletal_mesh() = default;
	
	/// Destructs a skeletal mesh.
	~skeletal_mesh() override = default;
	
	/// Sets the model with which this model instance is associated.
	/// @param model Shared pointer to the model.
	void set_model(std::shared_ptr<render::model> model);
	
	/// Overwrites the material of a model group for this model instance.
	/// @param index Index of a model group.
	/// @param material Pointer to the material which should overwrite the model group's material. A value of `nullptr` indicates the material will not be overwritten.
	void set_material(std::size_t index, std::shared_ptr<render::material> material);
	
	/// Resets all overwritten materials.
	void reset_materials();
	
	/// Returns the bounds of the skeletal mesh.
	[[nodiscard]] inline const aabb_type& get_bounds() const noexcept override
	{
		return m_bounds;
	}
	
	/// Returns the model of the model instance.
	[[nodiscard]] inline const std::shared_ptr<render::model>& get_model() const noexcept
	{
		return m_model;
	}
	
	void render(render::context& ctx) const override;
	
	/// Returns the skeleton of the skeletal mesh.
	[[nodiscard]] inline const skeleton* get_skeleton() const noexcept
	{
		return m_pose.get_skeleton();
	}
	
	/// Returns the pose of the skeletal mesh.
	[[nodiscard]] inline skeleton_pose& get_pose() noexcept
	{
		return m_pose;
	}

	/// @copydoc get_pose()
	[[nodiscard]] inline const skeleton_pose& get_pose() const noexcept
	{
		return m_pose;
	}
	
private:
	void update_bounds();
	void transformed() override;
	
	std::shared_ptr<render::model> m_model;
	mutable std::vector<render::operation> m_operations;
	aabb_type m_bounds{{0, 0, 0}, {0, 0, 0}};
	skeleton_pose m_pose;
};

} // namespace scene

#endif // ANTKEEPER_SCENE_SKELETAL_MESH_HPP
