/*
 * Copyright (C) 2020  Christopher J. Howard
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

#include "scene/camera.hpp"
#include "configuration.hpp"

using namespace vmq::operators;

static float4x4 interpolate_view(const camera* camera, const float4x4& x, const float4x4& y, float a)
{
	transform<float> transform = camera->get_transform_tween().interpolate(a);
	float3 forward = transform.rotation * global_forward;
	float3 up = transform.rotation * global_up;
	return vmq::look_at(transform.translation, transform.translation + forward, up);
}

static float4x4 interpolate_projection(const camera* camera, const float4x4& x, const float4x4& y, float a)
{
	if (camera->is_orthographic())
	{
		return vmq::ortho(
			camera->get_clip_left_tween().interpolate(a),
			camera->get_clip_right_tween().interpolate(a),
			camera->get_clip_bottom_tween().interpolate(a),
			camera->get_clip_top_tween().interpolate(a),
			camera->get_clip_near_tween().interpolate(a),
			camera->get_clip_far_tween().interpolate(a));
	}
	else
	{
		return vmq::perspective(
			camera->get_fov_tween().interpolate(a),
			camera->get_aspect_ratio_tween().interpolate(a),
			camera->get_clip_near_tween().interpolate(a),
			camera->get_clip_far_tween().interpolate(a));
	}
}

static float4x4 interpolate_view_projection(const camera* camera, const float4x4& x, const float4x4& y, float a)
{
	return camera->get_projection_tween().interpolate(a) * camera->get_view_tween().interpolate(a);
}

camera::camera():
	compositor(nullptr),
	composite_index(0),
	orthographic(true),
	clip_left(-1.0f),
	clip_right(1.0f),
	clip_bottom(-1.0f),
	clip_top(1.0f),
	clip_near(-1.0f),
	clip_far(1.0f),
	fov(vmq::half_pi<float>),
	aspect_ratio(1.0f),
	view(vmq::identity4x4<float>, std::bind(&interpolate_view, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3)),
	projection(vmq::identity4x4<float>, std::bind(&interpolate_projection, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3)),
	view_projection(vmq::identity4x4<float>, std::bind(&interpolate_view_projection, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3))
{}

float3 camera::project(const float3& object, const float4& viewport) const
{
	float4 result = view_projection[1] * float4{object[0], object[1], object[2], 1.0f};
	result[0] = (result[0] / result[3]) * 0.5f + 0.5f;
	result[1] = (result[1] / result[3]) * 0.5f + 0.5f;
	result[2] = (result[2] / result[3]) * 0.5f + 0.5f;
	
	result[0] = result[0] * viewport[2] + viewport[0];
	result[1] = result[1] * viewport[3] + viewport[1];
	
	return vmq::resize<3>(result);
}

float3 camera::unproject(const float3& window, const float4& viewport) const
{
	float4 result;
	result[0] = ((window[0] - viewport[0]) / viewport[2]) * 2.0f - 1.0f;
	result[1] = ((window[1] - viewport[1]) / viewport[3]) * 2.0f - 1.0f;
	result[2] = window[2] * 2.0f - 1.0f;
	result[3] = 1.0f;
	
	result = vmq::inverse(view_projection[1]) * result;

	return vmq::resize<3>(result) * (1.0f / result[3]);
}

void camera::set_perspective(float fov, float aspect_ratio, float clip_near, float clip_far)
{
	orthographic = false;

	this->fov[1] = fov;
	this->aspect_ratio[1] = aspect_ratio;
	this->clip_near[1] = clip_near;
	this->clip_far[1] = clip_far;

	projection[1] = vmq::perspective(fov, aspect_ratio, clip_near, clip_far);
	
	// Recalculate view-projection matrix
	view_projection[1] = projection[1] * view[1];
	
	// Recalculate view frustum
	view_frustum.set_matrix(view_projection[1]);
}

void camera::set_orthographic(float clip_left, float clip_right, float clip_bottom, float clip_top, float clip_near, float clip_far)
{
	orthographic = true;

	this->clip_left[1] = clip_left;
	this->clip_right[1] = clip_right;
	this->clip_bottom[1] = clip_bottom;
	this->clip_top[1] = clip_top;
	this->clip_near[1] = clip_near;
	this->clip_far[1] = clip_far;

	projection[1] = vmq::ortho(clip_left, clip_right, clip_bottom, clip_top, clip_near, clip_far);
	
	// Recalculate view-projection matrix
	view_projection[1] = projection[1] * view[1];
	
	// Recalculate view frustum
	view_frustum.set_matrix(view_projection[1]);
}

void camera::set_compositor(::compositor* compositor)
{
	this->compositor = compositor;
}

void camera::set_composite_index(int index)
{
	composite_index = index;
}

void camera::update_tweens()
{
	scene_object_base::update_tweens();
	clip_left.update();
	clip_right.update();
	clip_bottom.update();
	clip_top.update();
	clip_near.update();
	clip_far.update();
	fov.update();
	aspect_ratio.update();
	view.update();
	projection.update();
	view_projection.update();
}

void camera::transformed()
{
	// Recalculate view and view-projection matrices
	float3 forward = get_rotation() * global_forward;
	float3 up = get_rotation() * global_up;
	view[1] = vmq::look_at(get_translation(), get_translation() + forward, up);
	view_projection[1] = projection[1] * view[1];
	
	// Recalculate view frustum
	view_frustum.set_matrix(view_projection[1]);
}
