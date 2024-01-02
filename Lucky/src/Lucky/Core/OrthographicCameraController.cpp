#include "LuckyPch.h"
#include "OrthographicCameraController.h"

#include "Lucky/Application/Input.h"
#include "Lucky/Application/KeyCodes.h"
#include "Lucky/Core/Base.h"

namespace Lucky
{
	OrthographicCameraController::OrthographicCameraController(const Camera::Settings& settings)
	:	m_Settings(settings),
		m_Bounds({ -m_Settings.AspectRatio * m_Settings.ZoomLevel, m_Settings.AspectRatio * m_Settings.ZoomLevel, -m_Settings.ZoomLevel, m_Settings.ZoomLevel }),
		m_Camera(m_Bounds.Left, m_Bounds.Right, m_Bounds.Top, m_Bounds.Bottom)
	{
		LK_CORE_ASSERT(m_Settings.AspectRatio != 0, "Aspect ratio is not defined!")
	}

	void OrthographicCameraController::OnUpdate(Timestep ts)
	{
		LK_PROFILE_FUNCTION();
#ifndef __EMSCRIPTEN__
		if(Input::IsKeyPressed(Key::A))
#else
		if(Input::IsKeyPressed(Key::Q))
#endif
		{
			m_Settings.Position.x -= cos(glm::radians(m_Settings.Rotation)) *  m_Settings.TranslationSpeed * ts;
			m_Settings.Position.y -= sin(glm::radians(m_Settings.Rotation)) *  m_Settings.TranslationSpeed * ts;
		}	
		else
		if(Input::IsKeyPressed(Key::D))
		{
			m_Settings.Position.x += cos(glm::radians(m_Settings.Rotation)) *  m_Settings.TranslationSpeed * ts;
			m_Settings.Position.y += sin(glm::radians(m_Settings.Rotation)) *  m_Settings.TranslationSpeed * ts;
		}	

		if(Input::IsKeyPressed(Key::S))
		{
			m_Settings.Position.x -= -sin(glm::radians(m_Settings.Rotation)) *  m_Settings.TranslationSpeed * ts;
			m_Settings.Position.y -= cos(glm::radians(m_Settings.Rotation)) *  m_Settings.TranslationSpeed * ts;
		}	
		else 
#ifndef __EMSCRIPTEN__
		if(Input::IsKeyPressed(Key::W))
#else
		if(Input::IsKeyPressed(Key::Z))
#endif
		{
			m_Settings.Position.x += -sin(glm::radians(m_Settings.Rotation)) *  m_Settings.TranslationSpeed * ts;
			m_Settings.Position.y += cos(glm::radians(m_Settings.Rotation)) *  m_Settings.TranslationSpeed * ts;
		}	

		if(m_Settings.EnableRotation)
		{
			if(Input::IsKeyPressed(Key::E))
				m_Settings.Rotation -= m_Settings.RotationSpeed * ts;
			else
#ifndef __EMSCRIPTEN__
			if(Input::IsKeyPressed(Key::Q))
#else
			if(Input::IsKeyPressed(Key::A))
#endif
				m_Settings.Rotation += m_Settings.RotationSpeed * ts;

			if(m_Settings.Rotation > 180.0f)
				m_Settings.Rotation -= 360.0f;
			else if(m_Settings.Rotation <= -180.0f)
				m_Settings.Rotation += 360.0f;

			m_Camera.SetRotation(m_Settings.Rotation);
		}

		if(Input::IsKeyPressed(Key::Space))
		{
			m_Settings.Position = glm::vec3(0.0f);
			m_Settings.Rotation = 0.0f;
			m_Settings.ZoomLevel = 1.0f;
			CalculateView();
		}

		m_Camera.SetPosition(m_Settings.Position);
	}

	void OrthographicCameraController::OnEvent(Event &e)
	{
		LK_PROFILE_FUNCTION();
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<MouseScrolledEvent>(BIND_EVENT_FN(OrthographicCameraController::OnMouseScrolled));
		dispatcher.Dispatch<WindowResizeEvent>(BIND_EVENT_FN(OrthographicCameraController::OnWindowResized));
	}

	void OrthographicCameraController::OnImGuiRender()
	{
		LK_PROFILE_FUNCTION();
		ImGui::Begin("Orthographic Camera");
		ImGui::Text("Position: %.1f, %.1f", m_Settings.Position.x, m_Settings.Position.y);
		ImGui::Text("Zoom: %.1f", m_Settings.ZoomLevel);
		ImGui::Text("Rotation: %.1f", m_Settings.Rotation);
		ImGui::End();
	}

	void OrthographicCameraController::OnResize(float width, float height)
	{
		LK_PROFILE_FUNCTION();
		m_Settings.AspectRatio = width / height;
		CalculateView();
	}

	bool OrthographicCameraController::OnMouseScrolled(MouseScrolledEvent &e)
	{
		LK_PROFILE_FUNCTION();
		m_Settings.ZoomLevel -= e.GetYOffset() * m_Settings.ZoomSpeed;
		m_Settings.ZoomLevel = std::max(m_Settings.ZoomLevel, 0.25f);
		CalculateView();
		return false;
	}

	bool OrthographicCameraController::OnWindowResized(WindowResizeEvent &e)
	{
		LK_PROFILE_FUNCTION();
		OnResize((float)e.GetWidth(), (float)e.GetHeight());
		return false;
	}

	void OrthographicCameraController::CalculateView()
	{
		m_Bounds = { -m_Settings.AspectRatio * m_Settings.ZoomLevel, m_Settings.AspectRatio * m_Settings.ZoomLevel, -m_Settings.ZoomLevel, m_Settings.ZoomLevel };
		m_Camera.SetProjection(m_Bounds.Left, m_Bounds.Right, m_Bounds.Top, m_Bounds.Bottom);
	}
} // namespace Lucky
