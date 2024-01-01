#pragma once

#include "BaseCamera.h"

namespace Lucky
{
	class PerspectiveCamera : public BaseCamera
	{
	public:
		PerspectiveCamera(const CameraSettings& settings);

		void SetProjection(float fov, float aspectRatio, float zNear, float zFar);
	};
} // namespace Lucky
