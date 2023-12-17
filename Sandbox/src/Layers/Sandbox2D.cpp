#include "SandboxPch.h"
#include "Sandbox2D.h"

#include <imgui.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <Lucky/Platforms/OpenGL/OpenGLShader.h>

Sandbox2D::Sandbox2D()
    :   Layer("Sandbox2D"), m_SquareColor({0.0f, 0.407f, 0.48f, 1.0f})
{        
    auto& window = Lucky::Application::Get().GetWindow();

    Lucky::CameraSettings settings;
    settings.AspectRatio = (float)window.GetWidth() / (float)window.GetHeight();
    settings.EnableRotation = true;
    settings.TranslationSpeed = 2.0f;
    settings.RotationSpeed = 90.0f;
    settings.ZoomSpeed = 0.25f;

    m_CameraController = Lucky::CameraController::Create(Lucky::CameraType::Orthographic, settings);

    m_squareVA = Lucky::VertexArray::Create();
    m_squareVA->Bind();

    float squareVertices[3 * 4] = 
    {
        -0.5f,  -0.5f,  0.0f,
        0.5f,   -0.5f,  0.0f,
        0.5f,   0.5f,   0.0f,
        -0.5f,  0.5f,   0.0f
    };

    Lucky::Ref<Lucky::VertexBuffer> squareVB;
    squareVB = Lucky::VertexBuffer::Create(squareVertices, sizeof(squareVertices));
    squareVB->SetLayout({
        { Lucky::ShaderDataType::Float3, "a_Position" }
    });
    m_squareVA->AddVertexBuffer(squareVB);

    unsigned int squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
    Lucky::Ref<Lucky::IndexBuffer> squareIB;
    squareIB = Lucky::IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t));
    m_squareVA->SetIndexBuffer(squareIB);

    // Shaders
    m_ShaderLibrary.Load("assets/shaders/FlatColor.glsl");
}

Sandbox2D::~Sandbox2D()
{
}

void Sandbox2D::OnUpdate(Lucky::Timestep ts)
{
     // Background color
    Lucky::RenderCommand::SetClearColor({0.1f, 0.1f, 0.1f, 1.0f});
    Lucky::RenderCommand::Clear();

    m_CameraController->OnUpdate(ts);

    //Lucky::Renderer::BeginScene(m_Camera);
    Lucky::Renderer::BeginScene(m_CameraController);

    auto flatColorShader = m_ShaderLibrary.Get("FlatColor");

    std::dynamic_pointer_cast<Lucky::OpenGLShader>(flatColorShader)->Bind();
    std::dynamic_pointer_cast<Lucky::OpenGLShader>(flatColorShader)->UploadUniformFloat4("u_Color", m_SquareColor);

    Lucky::Renderer::Submit(flatColorShader, m_squareVA, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));

    Lucky::Renderer::EndScene();
}

void Sandbox2D::OnImGuiRender()
{
    m_CameraController->OnImGuiRender();

    ImGui::Begin("Squares");
    ImGui::ColorEdit4("", glm::value_ptr(m_SquareColor));
    ImGui::End();
}

void Sandbox2D::OnEvent(Lucky::Event &event)
{
    m_CameraController->OnEvent(event);
}
