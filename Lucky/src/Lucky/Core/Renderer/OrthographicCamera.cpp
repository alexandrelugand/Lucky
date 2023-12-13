#include "LuckyPch.h"
#include <glm/gtc/matrix_transform.hpp>

namespace Lucky
{
	OrthographicCamera::OrthographicCamera(float left, float right, float top, float bottom)
	{
		m_ProjectionMatrix = glm::ortho(left, right, bottom, top, -1.0f, 1.0f);
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}
} // namespace Lucky
