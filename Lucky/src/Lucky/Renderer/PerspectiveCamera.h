#pragma once

#include "Camera.h"

namespace Lucky
{
	class PerspectiveCamera : public Camera
	{
	public:
		PerspectiveCamera(const CameraSettings& settings);

		void SetProjection(float fov, float aspectRatio, float zNear, float zFar);
	};
} // namespace Lucky
