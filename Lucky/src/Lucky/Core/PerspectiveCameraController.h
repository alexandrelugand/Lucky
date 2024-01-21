#pragma once

#include "CameraController.h"
#include "Lucky/Renderer/PerspectiveCamera.h"
#include "Lucky/Core/Timestep.h"
#include "Lucky/Core/Events/Event.h"
#include "Lucky/Core/Events/ApplicationEvent.h"
#include "Lucky/Core/Events/MouseEvent.h"
#include "Lucky/Renderer/Camera.h"

namespace Lucky
{
	class PerspectiveCameraController : public CameraController
	{
	public:
		PerspectiveCameraController(const Camera::Settings& settings);
		virtual ~PerspectiveCameraController() = default;

		void OnUpdate(Timestep ts) override;
		void OnEvent(Event& e) override;
		void OnImGuiRender() override;

		BaseCamera& GetCamera() override { return m_Camera; }
		const BaseCamera& GetCamera() const override { return m_Camera; }
		const CameraBounds& GetBounds() const override { return m_Bounds; }

		float GetZoomLevel() const override { return m_Settings.Position.z; }
		void SetZoomLevel(float value) override { m_Settings.Position.z = value; }
		void OnResize(float width, float height) override;

	private:
		bool OnMouseScrolled(MouseScrolledEvent& e);
		bool OnWindowResized(WindowResizeEvent& e);

		Camera::Settings m_Settings;
		CameraBounds m_Bounds;
		PerspectiveCamera m_Camera;
	};
}
