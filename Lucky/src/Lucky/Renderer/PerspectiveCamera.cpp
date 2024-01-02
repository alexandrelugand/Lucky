#include "LuckyPch.h"
#include "PerspectiveCamera.h"

namespace Lucky
{
	PerspectiveCamera::PerspectiveCamera(const Camera::Settings& settings)
	{
		SetProjection(settings.PerspectiveFov, settings.AspectRatio, settings.PerspectiveNearClip, settings.PerspectiveFarClip);
	}

	void PerspectiveCamera::SetProjection(float fov, float aspectRatio, float zNear, float zFar)
	{
		LK_PROFILE_FUNCTION();
		m_ProjectionMatrix = glm::perspective(glm::radians(fov), aspectRatio, zNear, zFar);
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}
} // namespace Lucky
