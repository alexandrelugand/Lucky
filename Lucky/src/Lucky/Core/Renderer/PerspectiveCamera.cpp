#include "LuckyPch.h"
#include <glm/gtc/matrix_transform.hpp>

namespace Lucky
{
	PerspectiveCamera::PerspectiveCamera(float fov, float width, float height, float zNear, float zFar)
	{
		m_ProjectionMatrix = glm::perspectiveFov(fov, width, height, zNear, zFar);
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}
} // namespace Lucky
