#include "LuckyPch.h"
#include "SceneCamera.h"

namespace Lucky
{
	SceneCamera::SceneCamera(const CameraSettings& settings)
		: m_Settings(settings)
	{
		RecalculateProjection();
	}

	void SceneCamera::SetOrthographic(float size, float nearClip, float farClip)
	{
		m_Settings.Size = size;
		m_Settings.NearClip = nearClip;
		m_Settings.FarClip = farClip;
		RecalculateProjection();
	}

	void SceneCamera::SetViewportSize(uint32_t width, uint32_t height)
	{
		m_Settings.AspectRatio = (float)width / (float)height;
		RecalculateProjection();
	}

	void SceneCamera::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<MouseScrolledEvent>(BIND_EVENT_FN(SceneCamera::OnMouseScrolled));
	}

	void SceneCamera::RecalculateProjection()
	{
		float orthoLeft = -m_Settings.Size * m_Settings.AspectRatio * 0.5f;
		float orthoRight = m_Settings.Size * m_Settings.AspectRatio * 0.5f;
		float orthoBottom = -m_Settings.Size * 0.5f;
		float orthoTop = m_Settings.Size * 0.5f;

		m_Projection = glm::ortho(orthoLeft, orthoRight, orthoBottom, orthoTop, m_Settings.NearClip, m_Settings.FarClip);
	}

	bool SceneCamera::OnMouseScrolled(MouseScrolledEvent& e)
	{
		m_Settings.Size -= e.GetYOffset() * m_Settings.ZoomSpeed;
		RecalculateProjection();
		return false;
	}
}
