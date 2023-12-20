#pragma once
#include <Lucky.h>

class Sandbox3D : public Lucky::Layer
{
public:
    Sandbox3D();
    ~Sandbox3D();

    void OnUpdate(Lucky::Timestep ts) override;
    void OnImGuiRender() override;
    void OnEvent(Lucky::Event& event) override;

private:
    Lucky::Ref<Lucky::VertexArray> m_VertexArray;
    Lucky::Ref<Lucky::VertexArray> m_squareVA;
    Lucky::Ref<Lucky::Texture> m_Texture;
    Lucky::Ref<Lucky::Texture> m_TexturePlane;
    Lucky::Scope<Lucky::CameraController> m_CameraController;
    /*float m_triangleRotation = 0.0f;
    float m_triangleRotationSpeed = 45.0f;*/
    glm::vec4 m_SquareColor;
};