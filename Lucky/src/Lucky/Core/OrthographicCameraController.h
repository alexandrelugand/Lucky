#pragma once

#include "CameraController.h"
#include "Lucky/Renderer/OrthographicCamera.h"
#include "Lucky/Core/Timestep.h"
#include "Lucky/Core/Events/Event.h"
#include "Lucky/Core/Events/ApplicationEvent.h"
#include "Lucky/Core/Events/MouseEvent.h"
#include "Lucky/Renderer/Camera.h"

namespace Lucky
{
	class OrthographicCameraController : public CameraController
	{
	public:
		OrthographicCameraController(const Camera::Settings& settings);
		virtual ~OrthographicCameraController() = default;

		void OnUpdate(Timestep ts) override;
		void OnEvent(Event& e) override;
		void OnImGuiRender() override;

		BaseCamera& GetCamera() override { return m_Camera; }
		const BaseCamera& GetCamera() const override { return m_Camera; }
		const CameraBounds& GetBounds() const override { return m_Bounds; }

		float GetZoomLevel() const override { return m_Settings.ZoomLevel; }
		void SetZoomLevel(float value) override { m_Settings.ZoomLevel = value; CalculateView(); }
		void OnResize(float width, float height) override;

	private:
		bool OnMouseScrolled(MouseScrolledEvent& e);
		bool OnWindowResized(WindowResizeEvent& e);
		void CalculateView();

		Camera::Settings m_Settings;
		CameraBounds m_Bounds;
		OrthographicCamera m_Camera;
	};
} // namespace Lucky
