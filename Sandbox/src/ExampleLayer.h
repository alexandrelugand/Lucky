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
    Lucky::Ref<Lucky::VertexArray> m_VertexArray;
    Lucky::Ref<Lucky::VertexArray> m_squareVA;
    Lucky::ShaderLibrary m_ShaderLibrary;
    Lucky::Ref<Lucky::Texture> m_Texture;
    Lucky::Ref<Lucky::Texture> m_TexturePlane;
    Lucky::Scope<Lucky::CameraController> m_CameraController;
    /*float m_triangleRotation = 0.0f;
    float m_triangleRotationSpeed = 45.0f;*/
    glm::vec3 m_SquareColor;
};