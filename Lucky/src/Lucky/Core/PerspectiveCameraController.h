#pragma once

#include "CameraController.h"
#include "Lucky/Core/Renderer/PerspectiveCamera.h"
#include "Lucky/Core/Timestep.h"
#include "Lucky/Core/Events/Event.h"
#include "Lucky/Core/Events/ApplicationEvent.h"
#include "Lucky/Core/Events/MouseEvent.h"

namespace Lucky
{
	class PerspectiveCameraController : public CameraController
	{
	public:
		PerspectiveCameraController(const CameraSettings& settings);
		virtual ~PerspectiveCameraController() = default;

		void OnUpdate(Timestep ts) override;
		void OnEvent(Event& e) override;
		void OnImGuiRender() override;

		inline Camera& GetCamera() override { return m_Camera; }
		inline const Camera& GetCamera() const override { return m_Camera; }

	private:
		bool OnMouseScrolled(MouseScrolledEvent& e);
		bool OnWindowResized(WindowResizeEvent& e);

		CameraSettings m_Settings;
		PerspectiveCamera m_Camera;
		glm::vec3 m_Position;
		float m_Rotation;
	};
} // namespace Lucky
