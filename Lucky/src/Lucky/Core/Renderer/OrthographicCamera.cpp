#include "LuckyPch.h"
#include "OrthographicCamera.h"

namespace Lucky
{
	OrthographicCamera::OrthographicCamera(float left, float right, float top, float bottom)
	{
		SetProjection(left, right, top, bottom);
	}

	void OrthographicCamera::SetProjection(float left, float right, float top, float bottom)
	{
		m_ProjectionMatrix = glm::ortho(left, right, bottom, top, -1.0f, 1.0f);
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}
} // namespace Lucky
