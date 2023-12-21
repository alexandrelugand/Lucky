#include "SandboxPch.h"
#include "Sandbox2D.h"

Sandbox2D::Sandbox2D()
    :   Layer("Sandbox2D"), 
        m_SquareColor({0.0f, 0.407f, 0.48f, 1.0f})
{        
	auto& window = Lucky::Application::Get().GetWindow();

    Lucky::CameraSettings settings;
    settings.AspectRatio = (float)window.GetWidth() / (float)window.GetHeight();
    settings.EnableRotation = true;
    settings.TranslationSpeed = 2.0f;
    settings.RotationSpeed = 90.0f;
    settings.ZoomSpeed = 0.25f;

    m_CameraController = Lucky::CameraController::Create(Lucky::CameraType::Orthographic, settings);
}

void Sandbox2D::OnAttach()
{
	LK_PROFILE_FUNCTION();
	m_Texture = Lucky::Texture2D::Create("assets/textures/Checkerboard.png");
}

void Sandbox2D::OnDetach()
{
	LK_PROFILE_FUNCTION();
}

void Sandbox2D::OnUpdate(Lucky::Timestep ts)
{
	LK_PROFILE_FUNCTION();

	m_CameraController->OnUpdate(ts);

    // Prepare scene
	{
		LK_PROFILE_SCOPE("void Sandbox2D::OnUpdate() - Prepare");
		Lucky::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
		Lucky::RenderCommand::Clear();
	}

	// Render
	{
		LK_PROFILE_SCOPE("void Sandbox2D::OnUpdate() - Render");
		Lucky::Renderer2D::BeginScene(m_CameraController);

		Lucky::Renderer2D::DrawQuad({ 0.0f, 0.0f, -0.1f }, { 4.0f, 4.0f },m_Texture, 10.0f, { 0.8f, 1.0f, 0.8f, 1.0f});
		Lucky::Renderer2D::DrawRotateQuad(m_SquarePosition, m_SquareSize, m_SquareRotation, m_SquareColor);

		Lucky::Renderer2D::EndScene();
	}
}

void Sandbox2D::OnImGuiRender()
{
    m_CameraController->OnImGuiRender();

    ImGui::Begin("Square");
    ImGui::ColorEdit4("Color", glm::value_ptr(m_SquareColor));
    ImGui::DragFloat2("Position", glm::value_ptr(m_SquarePosition), 0.1f, -100.0f, 100.0f, "%.1f", ImGuiSliderFlags_AlwaysClamp);
    ImGui::DragFloat("Rotation", &m_SquareRotation, 1.0f, -360.0f, 360.0f, "%.1f", ImGuiSliderFlags_AlwaysClamp);
    ImGui::DragFloat2("Size", glm::value_ptr(m_SquareSize), 0.1f, 0.1f, 100.0f, "%.1f", ImGuiSliderFlags_AlwaysClamp);
    ImGui::End();
}

void Sandbox2D::OnEvent(Lucky::Event &event)
{
    m_CameraController->OnEvent(event);
}