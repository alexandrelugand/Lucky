#include "LuckyPch.h"
#include "SceneCamera.h"

namespace Lucky
{
	SceneCamera::SceneCamera(const Camera::Settings& settings)
		: Camera(settings)
	{
		RecalculateProjection();
	}

	void SceneCamera::SetViewportSize(uint32_t width, uint32_t height)
	{
		LK_CORE_ASSERT(width > 0 && height > 0);
		m_Settings.AspectRatio = (float)width / (float)height;
		RecalculateProjection();
	}

	void SceneCamera::SetPerspective(float fov, float nearClip, float farClip)
	{
		m_Settings.ProjectionType = ProjectionType::Perspective;
		m_Settings.PerspectiveFov = fov;
		m_Settings.PerspectiveNearClip = nearClip;
		m_Settings.PerspectiveFarClip = farClip;
		RecalculateProjection();
	}

	void SceneCamera::SetOrthographic(float size, float nearClip, float farClip)
	{
		m_Settings.ProjectionType = ProjectionType::Perspective;
		m_Settings.OrthographicSize = size;
		m_Settings.OrthographicNearClip = nearClip;
		m_Settings.OrthographicFarClip = farClip;
		RecalculateProjection();
	}

	void SceneCamera::RecalculateProjection()
	{
		if(m_Settings.ProjectionType == Camera::ProjectionType::Perspective)
		{
			m_Projection = glm::perspective(glm::radians(m_Settings.PerspectiveFov), m_Settings.AspectRatio, m_Settings.PerspectiveNearClip, m_Settings.PerspectiveFarClip);
		}
		else
		{
			float orthoLeft = -m_Settings.OrthographicSize * m_Settings.AspectRatio * 0.5f;
			float orthoRight = m_Settings.OrthographicSize * m_Settings.AspectRatio * 0.5f;
			float orthoBottom = -m_Settings.OrthographicSize * 0.5f;
			float orthoTop = m_Settings.OrthographicSize * 0.5f;
			m_Projection = glm::ortho(orthoLeft, orthoRight, orthoBottom, orthoTop, m_Settings.OrthographicNearClip, m_Settings.OrthographicFarClip);
		}
	}
}
