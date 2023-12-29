#pragma once

#include "Camera.h"

namespace Lucky
{
	class OrthographicCamera : public Camera
	{
	public:
		OrthographicCamera(float left, float right, float top, float bottom);

		void SetProjection(float left, float right, float top, float bottom);
	};
} // namespace Lucky
