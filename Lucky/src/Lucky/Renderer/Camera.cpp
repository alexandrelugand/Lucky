#include "LuckyPch.h"
#include "Camera.h"

namespace Lucky
{
	Camera::Camera(const Camera::Settings& settings)
		: m_Settings(settings), m_Projection(glm::mat4(1.0f))
	{
	}

	Camera::Camera(const Camera::Settings& settings, const glm::mat4& projection)
		: m_Settings(settings), m_Projection(projection)
	{
	}
}
