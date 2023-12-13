#pragma once

#include "Camera.h"

namespace Lucky
{
	class PerspectiveCamera : public Camera
	{
	public:
		PerspectiveCamera(float fov, float width, float height, float zNear, float zFar);
	};
} // namespace Lucky
