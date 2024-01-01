#pragma once

#include "Lucky/Core/CameraController.h"
#include "Lucky/Core/Events/Event.h"
#include "Lucky/Core/Events/MouseEvent.h"
#include "Lucky/Renderer/Camera.h"

namespace Lucky
{
	class SceneCamera : public Camera
	{
	public:
		SceneCamera(const CameraSettings& settings);
		virtual ~SceneCamera() = default;

		void SetOrthographic(float size, float nearClip, float farClip);
		void SetViewportSize(uint32_t width, uint32_t height);

		float GetOrthographicSize() const { return m_Settings.Size; }
		void SetOrthographicSize(float value) { m_Settings.Size = value; RecalculateProjection(); }

		void OnEvent(Event& e);

	private:
		void RecalculateProjection();
		bool OnMouseScrolled(MouseScrolledEvent& e);

		CameraSettings m_Settings;
	};
}
