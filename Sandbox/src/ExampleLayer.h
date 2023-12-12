#pragma once
#include <Lucky.h>
#include <glm/glm.hpp>

class ExampleLayer : public Lucky::Layer
{
public:
    ExampleLayer();
    ~ExampleLayer();

    void OnUpdate(Lucky::Timestep ts) override;
    void OnImGuiRender() override;
    void OnEvent(Lucky::Event& event) override;

private:
    bool OnKeyPressedEvent(Lucky::KeyPressedEvent& event);

    std::shared_ptr<Lucky::VertexArray> m_VertexArray;
    std::shared_ptr<Lucky::VertexArray> m_squareVA;
    std::shared_ptr<Lucky::Shader> m_Shader;
    std::shared_ptr<Lucky::Shader> m_FlatColorShader;
    Lucky::OrthographicCamera m_Camera;
    glm::vec3 m_CameraPosition;
    float m_CameraRotation;
    float m_CameraMoveSpeed;
    float m_CameraRotationSpeed;
    float m_triangleRotation;
    float m_triangleRotationSpeed;
    glm::vec3 m_SquareColor;
};