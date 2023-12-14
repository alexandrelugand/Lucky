#include "SandboxPch.h"
#include "ExampleLayer.h"

#include <imgui.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <Lucky/Platforms/OpenGL/OpenGLShader.h>

#ifdef __EMSCRIPTEN__
    #define GLSL_EXTENSION "wasm.glsl"
#else
    #define GLSL_EXTENSION "glsl"
#endif

ExampleLayer::ExampleLayer()
    :   m_CameraPosition({0.0f, 0.0f, 2.0f}),
        m_CameraRotation(0.0f), 
        m_CameraMoveSpeed(2.0f), 
        m_CameraRotationSpeed(90.0f),
        m_triangleRotation(0.0f),
        m_triangleRotationSpeed(45.0f),
        m_SquareColor({0.0f, 0.407f, 0.48f})
{        
    m_Camera = Lucky::Camera::Create(Lucky::CameraType::Perspective);
    m_VertexArray = Lucky::VertexArray::Create();
    m_VertexArray->Bind();

    float vertices[3 * 7] = 
    {
        -0.5f, -0.5f, 0.0f, 0.8f, 0.2f, 0.8f, 1.0f,
        0.5f, -0.5f, 0.0f, 0.2f, 0.3f, 0.8f, 1.0f,
        0.0f, 0.5f, 0.0f, 0.8f, 0.8f, 0.2f, 1.0f
    };
    Lucky::Ref<Lucky::VertexBuffer> vertexBuffer;
    vertexBuffer= Lucky::VertexBuffer::Create(vertices, sizeof(vertices));
    vertexBuffer->SetLayout({
        { Lucky::ShaderDataType::Float3, "a_Position" },
        { Lucky::ShaderDataType::Float4, "a_Color" }
    });
    m_VertexArray->AddVertexBuffer(vertexBuffer);

    unsigned int indices[3] = { 0, 1, 2 };
    Lucky::Ref<Lucky::IndexBuffer> indexBuffer;
    indexBuffer = Lucky::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t));
    m_VertexArray->SetIndexBuffer(indexBuffer);

    m_squareVA = Lucky::VertexArray::Create();
    m_squareVA->Bind();

    float squareVertices[5 * 4] = 
    {
        -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
        0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.0f, 1.0f, 1.0f,
        -0.5f, 0.5f, 0.0f, 0.0f, 1.0f
    };

    Lucky::Ref<Lucky::VertexBuffer> squareVB;
    squareVB = Lucky::VertexBuffer::Create(squareVertices, sizeof(squareVertices));
    squareVB->SetLayout({
        { Lucky::ShaderDataType::Float3, "a_Position" },
        { Lucky::ShaderDataType::Float2, "a_TexCoord" }
    });
    m_squareVA->AddVertexBuffer(squareVB);

    unsigned int squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
    Lucky::Ref<Lucky::IndexBuffer> squareIB;
    squareIB = Lucky::IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t));
    m_squareVA->SetIndexBuffer(squareIB);

    // Shaders
    m_Shader = Lucky::Shader::Create(fmt::format("assets/shaders/PositionColor.{0}", GLSL_EXTENSION));
    m_FlatColorShader = Lucky::Shader::Create(fmt::format("assets/shaders/FlatColor.{0}", GLSL_EXTENSION));
    m_TextureShader = Lucky::Shader::Create(fmt::format("assets/shaders/Texture.{0}", GLSL_EXTENSION));

    // Textures
    m_Texture = Lucky::Texture2D::Create("assets/textures/Checkerboard.png");
    m_TexturePlane = Lucky::Texture2D::Create("assets/textures/f14-tomcat.png");
}

ExampleLayer::~ExampleLayer()
{
}

void ExampleLayer::OnUpdate(Lucky::Timestep ts)
{
    if(Lucky::Input::IsKeyPressed(LK_KEY_LEFT))
        m_CameraPosition.x -= m_CameraMoveSpeed * ts;
    else if(Lucky::Input::IsKeyPressed(LK_KEY_RIGHT))
        m_CameraPosition.x += m_CameraMoveSpeed * ts;

    if(Lucky::Input::IsKeyPressed(LK_KEY_DOWN))
        m_CameraPosition.y -= m_CameraMoveSpeed * ts;
    else if(Lucky::Input::IsKeyPressed(LK_KEY_UP))
        m_CameraPosition.y += m_CameraMoveSpeed * ts;

#ifndef __EMSCRIPTEN__
    if(Lucky::Input::IsKeyPressed(LK_KEY_W))
#else
    if(Lucky::Input::IsKeyPressed(LK_KEY_Z))
#endif
        m_CameraPosition.z -= m_CameraMoveSpeed * ts;
    else if(Lucky::Input::IsKeyPressed(LK_KEY_S))
        m_CameraPosition.z += m_CameraMoveSpeed * ts;

    if(Lucky::Input::IsKeyPressed(LK_KEY_D))
        m_CameraRotation -= m_CameraRotationSpeed * ts;
    else
#ifndef __EMSCRIPTEN__
    if(Lucky::Input::IsKeyPressed(LK_KEY_A))
#else
    if(Lucky::Input::IsKeyPressed(LK_KEY_Q))
#endif
        m_CameraRotation += m_CameraRotationSpeed * ts;

    if(Lucky::Input::IsKeyPressed(LK_KEY_SPACE))
    {
        m_CameraPosition = glm::vec3({0.0f, 0.0f, 2.0f});
        m_CameraRotation = -0.0f;
    }

    if(m_CameraRotation > 360.0f || m_CameraRotation < -360.0f)
        m_CameraRotation = -0.0f;

    m_triangleRotation -= m_triangleRotationSpeed * ts;
    if(m_triangleRotation > 360.0f || m_triangleRotation < -360.0f)
        m_triangleRotation = -0.0f;

     // Background color
    Lucky::RenderCommand::SetClearColor({0.1f, 0.1f, 0.1f, 1.0f});
    Lucky::RenderCommand::Clear();

    m_Camera->SetPosition(m_CameraPosition);
    m_Camera->SetRotation(m_CameraRotation);

    Lucky::Renderer::BeginScene(m_Camera);

    static glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));
    std::dynamic_pointer_cast<Lucky::OpenGLShader>(m_FlatColorShader)->Bind();
    std::dynamic_pointer_cast<Lucky::OpenGLShader>(m_FlatColorShader)->UploadUniformFloat3("u_Color", m_SquareColor);

    for(int y = -10; y < 10; y++)
    {
        for(int x = -10; x < 10; x++)
        {
            glm::mat4 transform = glm::translate(glm::mat4(1.0f), glm::vec3(x * 0.11f, y * 0.11f, 0.0f)) * scale;
            Lucky::Renderer::Submit(m_FlatColorShader, m_squareVA, transform);
        }
    }

    m_Texture->Bind();
    Lucky::Renderer::Submit(m_TextureShader, m_squareVA, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));

    m_TexturePlane->Bind();
    Lucky::Renderer::Submit(m_TextureShader, m_squareVA, glm::rotate(glm::mat4(1.0f), glm::radians(-90.0f), glm::vec3(0.0f, 0.0f, 1.0f)));


    // Triangle   
    // glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), glm::radians(m_triangleRotation), glm::vec3(0.0f, 0.0f, 1.0f));
    // glm::mat4 transform = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f)) * rotation;
    // Lucky::Renderer::Submit(m_Shader, m_VertexArray, transform);

    Lucky::Renderer::EndScene();
}

void ExampleLayer::OnImGuiRender()
{
    ImGui::Begin("Camera");
    ImGui::Text("Position: %.1f, %.1f, %.1f", m_CameraPosition.x, m_CameraPosition.y, m_CameraPosition.z);
    ImGui::Text("Rotation: %.1f", -m_CameraRotation); //Inverse because rotation is in counterclockwise degrees
    ImGui::Separator();
    ImGui::ColorEdit3("Squares", glm::value_ptr(m_SquareColor));
    ImGui::End();
}

void ExampleLayer::OnEvent(Lucky::Event &event)
{
    Lucky::EventDispatcher dispatcher(event);
    dispatcher.Dispatch<Lucky::KeyPressedEvent>(BIND_EVENT_FN(ExampleLayer::OnKeyPressedEvent));
}

bool ExampleLayer::OnKeyPressedEvent(Lucky::KeyPressedEvent& event)
{
    LK_TRACE("Key press: {0}", event.GetKeyCode());
    return false;
}