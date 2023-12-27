#pragma once

#include "CameraController.h"
#include "Lucky/Core/Renderer/OrthographicCamera.h"
#include "Lucky/Core/Timestep.h"
#include "Lucky/Core/Events/Event.h"
#include "Lucky/Core/Events/ApplicationEvent.h"
#include "Lucky/Core/Events/MouseEvent.h"

namespace Lucky
{
	class OrthographicCameraController : public CameraController
	{
	public:
		OrthographicCameraController(const CameraSettings& settings);
		virtual ~OrthographicCameraController() = default;

		void OnUpdate(Timestep ts) override;
		void OnEvent(Event& e) override;
		void OnImGuiRender() override;

		Camera& GetCamera() override { return m_Camera; }
		const Camera& GetCamera() const override { return m_Camera; }
		const CameraBounds& GetBounds() const override { return m_Bounds; }

		float GetZoomLevel() const override { return m_ZoomLevel; }
		void SetZoomLevel(float value) override { m_ZoomLevel = value; CalculateView(); }

	private:
		bool OnMouseScrolled(MouseScrolledEvent& e);
		bool OnWindowResized(WindowResizeEvent& e);
		void CalculateView();

		CameraSettings m_Settings;
		float m_ZoomLevel = 1.0f;
		CameraBounds m_Bounds;
		OrthographicCamera m_Camera;
		glm::vec3 m_Position;
		float m_Rotation;
	};
} // namespace Lucky
