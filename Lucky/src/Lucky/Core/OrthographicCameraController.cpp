#include "LuckyPch.h"
#include "OrthographicCameraController.h"

#include "Lucky/Application/Input.h"
#include "Lucky/Application/KeyCodes.h"
#include "Lucky/Core/Core.h"

namespace Lucky
{
	OrthographicCameraController::OrthographicCameraController(const CameraSettings& settings)
	:	m_Settings(settings),
		m_Camera(-m_Settings.AspectRatio * m_ZoomLevel, m_Settings.AspectRatio * m_ZoomLevel, m_ZoomLevel, -m_ZoomLevel),
		m_Position(settings.Position),
		m_Rotation(settings.Rotation)
	{
		LK_CORE_ASSERT(settings.AspectRatio != 0, "Aspect ratio is not defined!")
	}

	void OrthographicCameraController::OnUpdate(Timestep ts)
	{
		#ifndef __EMSCRIPTEN__
		if(Input::IsKeyPressed(LK_KEY_A))
		#else
		if(Input::IsKeyPressed(LK_KEY_Q))
		#endif
			m_Position.x -= m_Settings.TranslationSpeed * ts;
		else
		if(Input::IsKeyPressed(LK_KEY_D))
			m_Position.x += m_Settings.TranslationSpeed * ts;

		if(Input::IsKeyPressed(LK_KEY_S))
			m_Position.y -= m_Settings.TranslationSpeed * ts;
		else 
		#ifndef __EMSCRIPTEN__
		if(Input::IsKeyPressed(LK_KEY_W))
		#else
		if(Input::IsKeyPressed(LK_KEY_Z))
		#endif
			m_Position.y += m_Settings.TranslationSpeed * ts;

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

			m_Camera.SetRotation(m_Rotation);
		}

		if(Input::IsKeyPressed(LK_KEY_SPACE))
		{
			m_Position = m_Settings.Position;
			m_Rotation = m_Settings.Rotation;
		}

		m_Camera.SetPosition(m_Position);
	}

	void OrthographicCameraController::OnEvent(Event &e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<MouseScrolledEvent>(BIND_EVENT_FN(OrthographicCameraController::OnMouseScrolled));
		dispatcher.Dispatch<WindowResizeEvent>(BIND_EVENT_FN(OrthographicCameraController::OnWindowResized));
	}

	void OrthographicCameraController::OnImGuiRender()
	{
		ImGui::Begin("Camera");
		ImGui::Text("Position: %.1f, %.1f", m_Position.x, m_Position.y);
		ImGui::Text("Zoom: %.1f", m_ZoomLevel);
		ImGui::Text("Rotation: %.1f", m_Rotation);
		ImGui::End();
	}

	bool OrthographicCameraController::OnMouseScrolled(MouseScrolledEvent &e)
	{
		m_ZoomLevel -= e.GetYOffset() * m_Settings.ZoomSpeed;
		m_ZoomLevel = std::max(m_ZoomLevel, 0.25f);
		m_Camera.SetProjection(-m_Settings.AspectRatio * m_ZoomLevel, m_Settings.AspectRatio * m_ZoomLevel, m_ZoomLevel, -m_ZoomLevel);
		return false;
	}

	bool OrthographicCameraController::OnWindowResized(WindowResizeEvent &e)
	{
		m_Settings.AspectRatio = (float)e.GetWidth() / (float)e.GetHeight();
		m_Camera.SetProjection(-m_Settings.AspectRatio * m_ZoomLevel, m_Settings.AspectRatio * m_ZoomLevel, m_ZoomLevel, -m_ZoomLevel);
		return false;
	}

} // namespace Lucky