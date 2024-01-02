#pragma once

#include "Lucky/Renderer/Camera.h"

namespace Lucky
{
	class SceneCamera : public Camera
	{
	public:
		SceneCamera(const Camera::Settings& settings);
		virtual ~SceneCamera() = default;

		void SetViewportSize(uint32_t width, uint32_t height);
		
		ProjectionType GetProjectionType() const { return m_Settings.ProjectionType; }
		void SetProjectionType(ProjectionType type) { m_Settings.ProjectionType = type; RecalculateProjection(); }

		void SetPerspective(float fov, float nearClip, float farClip);

		float GetPerspectiveFov() const { return m_Settings.PerspectiveFov; }
		void SetPerspectiveFov(float value) { m_Settings.PerspectiveFov = value; RecalculateProjection(); }
		float GetPerspectiveNearClip() const { return m_Settings.PerspectiveNearClip; }
		void SetPerspectiveNearClip(float value) { m_Settings.PerspectiveNearClip = value; RecalculateProjection(); }
		float GetPerspectiveFarClip() const { return m_Settings.PerspectiveFarClip; }
		void SetPerspectiveFarClip(float value) { m_Settings.PerspectiveFarClip = value; RecalculateProjection(); }

		void SetOrthographic(float size, float nearClip, float farClip);

		float GetOrthographicSize() const { return m_Settings.OrthographicSize; }
		void SetOrthographicSize(float value) { m_Settings.OrthographicSize = value; RecalculateProjection(); }
		float GetOrthographicNearClip() const { return m_Settings.OrthographicNearClip; }
		void SetOrthographicNearClip(float value) { m_Settings.OrthographicNearClip = value; RecalculateProjection(); }
		float GetOrthographicFarClip() const { return m_Settings.OrthographicFarClip; }
		void SetOrthographicFarClip(float value) { m_Settings.OrthographicFarClip = value; RecalculateProjection(); }

	private:
		void RecalculateProjection();
	};
}
