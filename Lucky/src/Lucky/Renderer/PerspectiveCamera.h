#pragma once

#include "BaseCamera.h"
#include "Camera.h"

namespace Lucky
{
	class PerspectiveCamera : public BaseCamera
	{
	public:
		PerspectiveCamera(const Camera::Settings& settings);

		void SetProjection(float fov, float aspectRatio, float zNear, float zFar);
	};
} // namespace Lucky
