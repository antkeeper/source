// SPDX-FileCopyrightText: 2025 C. J. Howard
// SPDX-License-Identifier: GPL-3.0-or-later

#include "game/systems/reproductive-system.hpp"
#include "game/components/ovary-component.hpp"
#include "game/components/scene-component.hpp"
#include "game/components/pose-component.hpp"
#include "game/components/rigid-body-component.hpp"
#include "game/components/egg-component.hpp"
#include "game/components/ant-genome-component.hpp"
#include "game/systems/physics-system.hpp"
#include <engine/math/functions.hpp>
#include <engine/math/functions.hpp>
#include <engine/scene/static-mesh.hpp>
#include <engine/debug/log.hpp>

reproductive_system::reproductive_system(entity::registry& registry):
	updatable_system(registry)
{}

void reproductive_system::update(float, float dt)
{
	auto ovary_group = m_registry.group<ovary_component>(entt::get<ant_genome_component, rigid_body_component, scene_component, pose_component>);
	for (auto entity_id: ovary_group)
	{
		auto& ovary = ovary_group.get<ovary_component>(entity_id);
		
		// Produce eggs
		if (ovary.egg_count < ovary.egg_capacity)
		{
			ovary.elapsed_egg_production_time += dt * m_time_scale;
			if (ovary.elapsed_egg_production_time >= ovary.egg_production_duration)
			{
				ovary.egg_count += static_cast<std::uint16_t>(ovary.elapsed_egg_production_time / ovary.egg_production_duration);
				ovary.egg_count = std::min(ovary.egg_count, ovary.egg_capacity);
				ovary.elapsed_egg_production_time = math::fract(ovary.elapsed_egg_production_time);
			}
		}
		
		// Oviposit egg
		if (ovary.ovipositor_egg_eid != entt::null || (ovary.ovipositing && ovary.egg_count))
		{
			// Get transform of ovipositor
			const auto& ovipositor_rigid_body = *ovary_group.get<rigid_body_component>(entity_id).body;
			const auto& ovipositor_pose = ovary_group.get<pose_component>(entity_id);
			const auto ovipositor_transform = ovipositor_rigid_body.get_transform() * ovipositor_pose.current_pose.get_absolute_transform(ovary.ovipositor_bone);
			
			// Advance oviposition time
			if (ovary.ovipositing)
			{
				ovary.elapsed_oviposition_time += dt * m_time_scale;
			}
			else
			{
				ovary.elapsed_oviposition_time -= dt * m_time_scale;
				ovary.elapsed_oviposition_time = std::max(0.0f, ovary.elapsed_oviposition_time);
			}
			
			// Determine position and orientation of egg
			const float t = std::min(ovary.elapsed_oviposition_time / ovary.oviposition_duration, 1.0f);
			auto egg_transform = ovipositor_transform;
			egg_transform.translation = egg_transform * math::lerp(ovary.oviposition_path.a, ovary.oviposition_path.b, t);
			
			if (ovary.ovipositor_egg_eid == entt::null)
			{
				// Get genome of parent entity
				const auto& parent_genome = ovary_group.get<ant_genome_component>(entity_id);
				
				// Get scene component of ovipositing entity
				const auto& ovipositor_scene = ovary_group.get<scene_component>(entity_id);
				
				
				// Construct egg rigid body
				auto egg_rigid_body = std::make_unique<physics::rigid_body>();
				egg_rigid_body->set_mass(0.0f);
				egg_rigid_body->set_transform(egg_transform);
				egg_rigid_body->set_previous_transform(egg_transform);
				
				// Construct egg scene object
				auto egg_scene_object = std::make_shared<scene::static_mesh>(parent_genome.genome->egg->phenes.front().model);
				
				// Construct egg entity
				ovary.ovipositor_egg_eid = m_registry.create();
				m_registry.emplace<rigid_body_component>(ovary.ovipositor_egg_eid, std::move(egg_rigid_body));
				m_registry.emplace<scene_component>(ovary.ovipositor_egg_eid, std::move(egg_scene_object), ovipositor_scene.layer_mask);
				m_registry.emplace<ant_genome_component>(ovary.ovipositor_egg_eid, parent_genome);
			}
			else
			{
				// Update position of egg rigid body
				auto& egg_rigid_body = *m_registry.get<rigid_body_component>(ovary.ovipositor_egg_eid).body;
				egg_rigid_body.set_transform(egg_transform);
			}
			
			if (ovary.elapsed_oviposition_time >= ovary.oviposition_duration)
			{
				// Place egg
				auto& egg_rigid_body = *m_registry.get<rigid_body_component>(ovary.ovipositor_egg_eid).body;
				const auto oviposition_ray = geom::ray<float, 3>{egg_transform.translation, egg_transform.rotation * math::fvec3{0, 0, -1}};
				if (auto trace = m_physics_system->trace(oviposition_ray, ovary.ovipositor_egg_eid, ~std::uint32_t{0}))
				{
					egg_transform.translation = oviposition_ray.extrapolate(std::get<1>(*trace));
					egg_transform.rotation = math::normalize(math::rotation(egg_transform.rotation * math::fvec3{0, 1, 0}, std::get<3>(*trace)) * egg_transform.rotation);
					egg_rigid_body.set_transform(egg_transform);
					
					// Get genome of egg
					const auto& genome = *m_registry.get<ant_genome_component>(ovary.ovipositor_egg_eid).genome;
					
					// Construct egg component
					m_registry.emplace<egg_component>(ovary.ovipositor_egg_eid, genome.egg->phenes.front().incubation_period, 0.0f);
					
					// Oviposition complete
					ovary.ovipositing = false;
					ovary.elapsed_oviposition_time = 0.0f;
					--ovary.egg_count;
					ovary.ovipositor_egg_eid = entt::null;
				}
			}
		}
	}
}
