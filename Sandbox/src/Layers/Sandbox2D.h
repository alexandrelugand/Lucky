#pragma once
#include <Lucky.h>

class Sandbox2D : public Lucky::Layer
{
public:
    Sandbox2D();

	void OnAttach() override;
	void OnDetach() override;
    void OnUpdate(Lucky::Timestep ts) override;
    void OnImGuiRender() override;
    void OnEvent(Lucky::Event& event) override;

private:
    Lucky::Scope<Lucky::CameraController> m_CameraController;
    glm::vec2 m_SquarePosition = { 0.0f, 0.0f };
    glm::vec2 m_SquareSize = { 1.0f, 1.0f };
    float m_SquareRotation = 0.0f;
    glm::vec4 m_SquareColor;
    Lucky::Ref<Lucky::Texture> m_Texture;
};
