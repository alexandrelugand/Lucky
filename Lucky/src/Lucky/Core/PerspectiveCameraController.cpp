#include "LuckyPch.h"
#include "PerspectiveCameraController.h"

#include "Lucky/Application/Input.h"
#include "Lucky/Application/KeyCodes.h"
#include "Lucky/Core/Core.h"

namespace Lucky
{
	PerspectiveCameraController::PerspectiveCameraController(const CameraSettings &settings)
	:	m_Settings(settings),
		m_Camera(settings),
		m_Position(settings.Position),
		m_Rotation(settings.Rotation)
	{
		LK_CORE_ASSERT(settings.Fov != 0, "Fov is not defined!")
		LK_CORE_ASSERT(settings.AspectRatio != 0, "Aspect ratio is not defined!")
		LK_CORE_ASSERT(settings.ZNear != 0, "ZNear is not defined!")
		LK_CORE_ASSERT(settings.ZFar != 0, "ZFar is not defined!")
	}

	void PerspectiveCameraController::OnUpdate(Timestep ts)
	{
		#ifndef __EMSCRIPTEN__
		if(Input::IsKeyPressed(LK_KEY_A))
		#else
		if(Input::IsKeyPressed(LK_KEY_Q))
		#endif
		{
			m_Position.x -= cos(glm::radians(m_Rotation)) *  m_Settings.TranslationSpeed * ts;
			m_Position.y -= sin(glm::radians(m_Rotation)) *  m_Settings.TranslationSpeed * ts;
		}	
		else
		if(Input::IsKeyPressed(LK_KEY_D))
		{
			m_Position.x += cos(glm::radians(m_Rotation)) *  m_Settings.TranslationSpeed * ts;
			m_Position.y += sin(glm::radians(m_Rotation)) *  m_Settings.TranslationSpeed * ts;
		}	

		if(Input::IsKeyPressed(LK_KEY_S))
		{
			m_Position.x -= -sin(glm::radians(m_Rotation)) *  m_Settings.TranslationSpeed * ts;
			m_Position.y -= cos(glm::radians(m_Rotation)) *  m_Settings.TranslationSpeed * ts;
		}	
		else 
		#ifndef __EMSCRIPTEN__
		if(Input::IsKeyPressed(LK_KEY_W))
		#else
		if(Input::IsKeyPressed(LK_KEY_Z))
		#endif
		{
			m_Position.x += -sin(glm::radians(m_Rotation)) *  m_Settings.TranslationSpeed * ts;
			m_Position.y += cos(glm::radians(m_Rotation)) *  m_Settings.TranslationSpeed * ts;
		}	

		if(m_Settings.EnableRotation)
		{
			if(Input::IsKeyPressed(LK_KEY_E))
				m_Rotation -= m_Settings.RotationSpeed * ts;
			else
			#ifndef __EMSCRIPTEN__
			if(Input::IsKeyPressed(LK_KEY_Q))
			#else
			if(Input::IsKeyPressed(LK_KEY_A))
			#endif
				m_Rotation += m_Settings.RotationSpeed * ts;

			if(m_Rotation > 180.0f)
				m_Rotation -= 360.0f;
			else if(m_Rotation <= -180.0f)
				m_Rotation += 360.0f;

			m_Camera.SetRotation(m_Rotation);
		}

		if(Input::IsKeyPressed(LK_KEY_SPACE))
		{
			m_Position = m_Settings.Position;
			m_Rotation = m_Settings.Rotation;
		}

		m_Camera.SetPosition(m_Position);
	}

	void PerspectiveCameraController::OnEvent(Event &e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<MouseScrolledEvent>(BIND_EVENT_FN(PerspectiveCameraController::OnMouseScrolled));
		dispatcher.Dispatch<WindowResizeEvent>(BIND_EVENT_FN(PerspectiveCameraController::OnWindowResized));
	}

	void PerspectiveCameraController::OnImGuiRender()
	{
		ImGui::Begin("Perspective Camera");
		ImGui::Text("Position: %.1f, %.1f", m_Position.x, m_Position.y);
		ImGui::Text("Zoom: %.1f", m_Position.z);
		ImGui::Text("Rotation: %.1f", m_Rotation);
		ImGui::End();
	}

	bool PerspectiveCameraController::OnMouseScrolled(MouseScrolledEvent &e)
	{
		m_Position.z -= e.GetYOffset() * m_Settings.ZoomSpeed;
		m_Position.z = std::max(m_Position.z, 0.2f);
		return false;
	}

	bool PerspectiveCameraController::OnWindowResized(WindowResizeEvent &e)
	{
		m_Camera.SetProjection(m_Settings.Fov, (float)e.GetWidth() / (float)e.GetHeight(), m_Settings.ZNear, m_Settings.ZFar);
		return false;
	}

} // namespace Lucky
