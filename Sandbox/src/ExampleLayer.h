#pragma once
#include <Lucky.h>

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

    Lucky::Ref<Lucky::VertexArray> m_VertexArray;
    Lucky::Ref<Lucky::VertexArray> m_squareVA;
    Lucky::ShaderLibrary m_ShaderLibrary;
    Lucky::Ref<Lucky::Texture> m_Texture;
    Lucky::Ref<Lucky::Texture> m_TexturePlane;
    Lucky::Scope<Lucky::Camera> m_Camera;
    glm::vec3 m_CameraPosition;
    float m_CameraRotation;
    float m_CameraMoveSpeed;
    float m_CameraRotationSpeed;
    float m_triangleRotation;
    float m_triangleRotationSpeed;
    glm::vec3 m_SquareColor;
};