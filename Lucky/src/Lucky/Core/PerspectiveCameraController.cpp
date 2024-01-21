#include "LuckyPch.h"
#include "PerspectiveCameraController.h"

#include "Lucky/Application/Input.h"
#include "Lucky/Application/KeyCodes.h"
#include "Lucky/Core/Base.h"

namespace Lucky
{
	PerspectiveCameraController::PerspectiveCameraController(const Camera::Settings &settings)
	:	m_Settings(settings),
		m_Camera(settings)
	{
		LK_CORE_ASSERT(m_Settings.PerspectiveFov != 0, "Fov is not defined!")
			LK_CORE_ASSERT(settings.AspectRatio != 0, "Aspect ratio is not defined!")
			LK_CORE_ASSERT(settings.PerspectiveNearClip != 0, "ZNear is not defined!")
			LK_CORE_ASSERT(settings.PerspectiveFarClip != 0, "ZFar is not defined!")

		m_Settings.Position.z = m_Settings.ZoomLevel;

		float frustumHeight = 2.0f * abs(m_Settings.Position.z) * tan(m_Settings.PerspectiveFov * 0.5f * (glm::pi<float>() / 180.0f));
		float frustumWidth = frustumHeight * m_Settings.AspectRatio;
		m_Bounds = { 0.0f, frustumWidth, 0.0f, frustumHeight };
	}

	void PerspectiveCameraController::OnUpdate(Timestep ts)
	{
		LK_PROFILE_FUNCTION();
#ifndef __EMSCRIPTEN__
		if(Input::IsKeyPressed(LK_KEY_A))
#else
		if(Input::IsKeyPressed(LK_KEY_Q))
#endif
		{
			m_Settings.Position.x -= cos(glm::radians(m_Settings.Rotation)) *  m_Settings.TranslationSpeed * ts;
			m_Settings.Position.y -= sin(glm::radians(m_Settings.Rotation)) *  m_Settings.TranslationSpeed * ts;
		}	
		else
		if(Input::IsKeyPressed(LK_KEY_D))
		{
			m_Settings.Position.x += cos(glm::radians(m_Settings.Rotation)) *  m_Settings.TranslationSpeed * ts;
			m_Settings.Position.y += sin(glm::radians(m_Settings.Rotation)) *  m_Settings.TranslationSpeed * ts;
		}	

		if(Input::IsKeyPressed(LK_KEY_S))
		{
			m_Settings.Position.x -= -sin(glm::radians(m_Settings.Rotation)) *  m_Settings.TranslationSpeed * ts;
			m_Settings.Position.y -= cos(glm::radians(m_Settings.Rotation)) *  m_Settings.TranslationSpeed * ts;
		}	
		else 
#ifndef __EMSCRIPTEN__
		if(Input::IsKeyPressed(LK_KEY_W))
#else
		if(Input::IsKeyPressed(LK_KEY_Z))
#endif
		{
			m_Settings.Position.x += -sin(glm::radians(m_Settings.Rotation)) *  m_Settings.TranslationSpeed * ts;
			m_Settings.Position.y += cos(glm::radians(m_Settings.Rotation)) *  m_Settings.TranslationSpeed * ts;
		}	

		if(m_Settings.EnableRotation)
		{
			if(Input::IsKeyPressed(LK_KEY_E))
				m_Settings.Rotation -= m_Settings.RotationSpeed * ts;
			else
#ifndef __EMSCRIPTEN__
			if(Input::IsKeyPressed(LK_KEY_Q))
#else
			if(Input::IsKeyPressed(LK_KEY_A))
#endif
				m_Settings.Rotation += m_Settings.RotationSpeed * ts;

			if(m_Settings.Rotation > 180.0f)
				m_Settings.Rotation -= 360.0f;
			else if(m_Settings.Rotation <= -180.0f)
				m_Settings.Rotation += 360.0f;

			m_Camera.SetRotation(m_Settings.Rotation);
		}

		if(Input::IsKeyPressed(LK_KEY_SPACE))
		{
			m_Settings.Position = m_Settings.Position;
			m_Settings.Rotation = m_Settings.Rotation;
			m_Settings.Position.z = m_Settings.ZoomLevel;
		}

		m_Camera.SetPosition(m_Settings.Position);
	}

	void PerspectiveCameraController::OnEvent(Event &e)
	{
		LK_PROFILE_FUNCTION();
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<MouseScrolledEvent>(BIND_EVENT_FN(PerspectiveCameraController::OnMouseScrolled));
		dispatcher.Dispatch<WindowResizeEvent>(BIND_EVENT_FN(PerspectiveCameraController::OnWindowResized));
	}

	void PerspectiveCameraController::OnImGuiRender()
	{
		LK_PROFILE_FUNCTION();
		ImGui::Begin("Perspective Camera");
		ImGui::Text("Position: %.1f, %.1f", m_Settings.Position.x, m_Settings.Position.y);
		ImGui::Text("Zoom: %.1f", m_Settings.Position.z);
		ImGui::Text("Rotation: %.1f", m_Settings.Rotation);
		ImGui::End();
	}

	void PerspectiveCameraController::OnResize(float width, float height)
	{
		m_Camera.SetProjection(m_Settings.PerspectiveFov, width / height, m_Settings.PerspectiveNearClip, m_Settings.PerspectiveFarClip);
	}

	bool PerspectiveCameraController::OnMouseScrolled(MouseScrolledEvent &e)
	{
		LK_PROFILE_FUNCTION();
		m_Settings.Position.z -= e.GetYOffset() * m_Settings.ZoomSpeed;
		m_Settings.Position.z = std::max(m_Settings.Position.z, 0.2f);
		return false;
	}

	bool PerspectiveCameraController::OnWindowResized(WindowResizeEvent &e)
	{
		LK_PROFILE_FUNCTION();
		OnResize((float)e.GetWidth(), (float)e.GetHeight());
		return false;
	}

}
