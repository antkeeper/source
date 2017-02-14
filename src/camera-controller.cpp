/*
 * Copyright (C) 2017  Christopher J. Howard
 *
 * This file is part of Antkeeper Source Code.
 *
 * Antkeeper Source Code is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Antkeeper Source Code is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Antkeeper Source Code.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "camera-controller.hpp"

CameraController::CameraController():
	camera(nullptr)
{}

SurfaceCameraController::SurfaceCameraController():
	elevationRotation(1, 0, 0, 0),
	azimuthRotation(1, 0, 0, 0),
	rotation(1, 0, 0, 0),
	targetElevationRotation(1, 0, 0, 0),
	targetAzimuthRotation(1, 0, 0, 0),
	targetRotation(1, 0, 0, 0)
{}

SurfaceCameraController::~SurfaceCameraController()
{}

void SurfaceCameraController::update(float dt)
{
	float interpolationFactor = 0.25f / (1.0 / 60.0f) * dt;

	// Calculate rotation and target rotation quaternions
	//rotation = azimuthRotation * elevationRotation;
	targetRotation = targetAzimuthRotation * targetElevationRotation;
	
	// Calculate target translation
	targetTranslation = targetFocalPoint + targetRotation * Vector3(0.0f, 0.0f, targetFocalDistance);
	
	// Interpolate rotation
	//rotation = glm::mix(rotation, targetRotation, interpolationFactor);
	
	// Interpolate angles
	setElevation(glm::mix(elevation, targetElevation, interpolationFactor));
	setAzimuth(glm::mix(azimuth, targetAzimuth, interpolationFactor));
	
	// Calculate rotation
	rotation = azimuthRotation * elevationRotation;
	
	// Interpolate focal point and focal distance
	focalPoint = glm::mix(focalPoint, targetFocalPoint, interpolationFactor);
	focalDistance = glm::mix(focalDistance, targetFocalDistance, interpolationFactor);
	
	// Caluclate translation
	translation = focalPoint + rotation * Vector3(0.0f, 0.0f, focalDistance);
	
	/*
	// Recalculate azimuth
	azimuthRotation = rotation;
	azimuthRotation.x = 0.0f;
	azimuthRotation.z = 0.0f;
	azimuthRotation = glm::normalize(azimuthRotation);
	azimuth = 2.0f * std::acos(azimuthRotation.w);
	
	// Recalculate elevation
	elevationRotation = rotation;
	elevationRotation.y = 0.0f;
	elevationRotation.z = 0.0f;
	elevationRotation = glm::normalize(elevationRotation);
	elevation = 2.0f * std::acos(elevationRotation.w);
	*/
	
	// Update camera
	if (camera != nullptr)
	{
		camera->lookAt(translation, focalPoint, Vector3(0.0f, 1.0f, 0.0f));
	}
}

void SurfaceCameraController::move(Vector2 direction)
{
	targetFocalPoint += azimuthRotation * Vector3(direction.x, 0.0f, direction.y);
}

void SurfaceCameraController::rotate(float angle)
{
	setTargetAzimuth(targetAzimuth + angle);
}

void SurfaceCameraController::zoom(float distance)
{
	setTargetFocalDistance(targetFocalDistance - distance);
}

void SurfaceCameraController::setFocalPoint(const Vector3& point)
{
	focalPoint = point;
}

void SurfaceCameraController::setFocalDistance(float distance)
{
	focalDistance = distance;
}

void SurfaceCameraController::setElevation(float angle)
{
	elevation = angle;
	elevationRotation = glm::angleAxis(elevation, Vector3(-1.0f, 0.0f, 0.0f));
}

void SurfaceCameraController::setAzimuth(float angle)
{
	azimuth = angle;
	azimuthRotation = glm::angleAxis(azimuth, Vector3(0.0f, 1.0f, 0.0f));
}

void SurfaceCameraController::setTargetFocalPoint(const Vector3& point)
{
	targetFocalPoint = point;
}

void SurfaceCameraController::setTargetFocalDistance(float distance)
{
	targetFocalDistance = distance;
}

void SurfaceCameraController::setTargetElevation(float angle)
{
	targetElevation = angle;
	targetElevationRotation = glm::angleAxis(targetElevation, Vector3(-1.0f, 0.0f, 0.0f));
}

void SurfaceCameraController::setTargetAzimuth(float angle)
{
	targetAzimuth = angle;
	targetAzimuthRotation = glm::angleAxis(targetAzimuth, Vector3(0.0f, 1.0f, 0.0f));
}

TunnelCameraController::TunnelCameraController()
{}

TunnelCameraController::~TunnelCameraController()
{}

void TunnelCameraController::update(float dt)
{
	
}