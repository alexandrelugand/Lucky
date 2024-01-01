#pragma once

#include "BaseCamera.h"

namespace Lucky
{
	class OrthographicCamera : public BaseCamera
	{
	public:
		OrthographicCamera(float left, float right, float top, float bottom);

		void SetProjection(float left, float right, float top, float bottom);
	};
} // namespace Lucky
