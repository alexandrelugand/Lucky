#include "SandboxPch.h"
#include <imgui.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <Lucky/Platforms/OpenGL/OpenGLShader.h>

ExampleLayer::ExampleLayer()
    :   m_Camera(-1.6f, 1.6f, 0.9f, -0.9f), 
        m_CameraPosition({0.0f, 0.0f, 0.0f}),
        m_CameraRotation(0.0f), 
        m_CameraMoveSpeed(2.0f), 
        m_CameraRotationSpeed(90.0f),
        m_triangleRotation(0.0f),
        m_triangleRotationSpeed(45.0f),
        m_SquareColor({0.75f, 0.75f, 0.75f})
{        
    m_VertexArray.reset(Lucky::VertexArray::Create());
    m_VertexArray->Bind();

    float vertices[3 * 7] = 
    {
        -0.5f, -0.5f, 0.0f, 0.8f, 0.2f, 0.8f, 1.0f,
        0.5f, -0.5f, 0.0f, 0.2f, 0.3f, 0.8f, 1.0f,
        0.0f, 0.5f, 0.0f, 0.8f, 0.8f, 0.2f, 1.0f
    };
    std::shared_ptr<Lucky::VertexBuffer> vertexBuffer;
    vertexBuffer.reset(Lucky::VertexBuffer::Create(vertices, sizeof(vertices)));
    vertexBuffer->SetLayout({
        { Lucky::ShaderDataType::Float3, "a_Position" },
        { Lucky::ShaderDataType::Float4, "a_Color" }
    });
    m_VertexArray->AddVertexBuffer(vertexBuffer);

    unsigned int indices[3] = { 0, 1, 2 };
    std::shared_ptr<Lucky::IndexBuffer> indexBuffer;
    indexBuffer.reset(Lucky::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
    m_VertexArray->SetIndexBuffer(indexBuffer);

#ifdef __EMSCRIPTEN__
    std::string vertexSrc = "#version 300 es\n";
#else
    std::string vertexSrc = "#version 410\n";
#endif
    vertexSrc += R"(
        layout(location = 0) in vec3 a_Position;
        layout(location = 1) in vec4 a_Color;
        out vec3 v_Position;
        out vec4 v_Color;

        uniform mat4 u_ViewProjection;
        uniform mat4 u_Transform;

        void main()
        {
            v_Position = a_Position;
            v_Color = a_Color;
            gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
        }
    )";

#ifdef __EMSCRIPTEN__
    std::string fragmentSrc = "#version 300 es\n";
    fragmentSrc += "precision mediump float;\n";
#else
    std::string fragmentSrc = "#version 410\n";
#endif
    fragmentSrc += R"(
        layout(location = 0) out vec4 color;
        in vec3 v_Position;
        in vec4 v_Color;

        void main()
        {
            color = vec4(v_Position * 0.5 + 0.5, 1.0);
            color = v_Color;
        }
    )";

    m_Shader.reset(Lucky::Shader::Create(vertexSrc, fragmentSrc));

    m_squareVA.reset(Lucky::VertexArray::Create());
    m_squareVA->Bind();

    float squareVertices[4 * 3] = 
    {
        -0.5f, -0.5f, 0.0f,
        0.5f, -0.5f, 0.0f, 
        0.5f, 0.5f, 0.0f,
        -0.5f, 0.5f, 0.0f
    };
    std::shared_ptr<Lucky::VertexBuffer> squareVB;
    squareVB.reset(Lucky::VertexBuffer::Create(squareVertices, sizeof(squareVertices)));
    squareVB->SetLayout({
        { Lucky::ShaderDataType::Float3, "a_Position" }
    });
    m_squareVA->AddVertexBuffer(squareVB);

    unsigned int squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
    std::shared_ptr<Lucky::IndexBuffer> squareIB;
    squareIB.reset(Lucky::IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));
    m_squareVA->SetIndexBuffer(squareIB);

#ifdef __EMSCRIPTEN__
    vertexSrc = "#version 300 es\n";
#else
    vertexSrc = "#version 410\n";
#endif
    vertexSrc += R"(
        layout(location = 0) in vec3 a_Position;
        out vec3 v_Position;

        uniform mat4 u_ViewProjection;
        uniform mat4 u_Transform;

        void main()
        {
            v_Position = a_Position;
            gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
        }
    )";

#ifdef __EMSCRIPTEN__
    fragmentSrc = "#version 300 es\n";
    fragmentSrc += "precision mediump float;\n";
#else
    fragmentSrc = "#version 410\n";
#endif
    fragmentSrc += R"(
        layout(location = 0) out vec4 color;
        in vec3 v_Position;

        uniform vec3 u_Color;

        void main()
        {
            color = vec4(u_Color, 1.0);
        }
    )";

    m_FlatColorShader.reset(Lucky::Shader::Create(vertexSrc, fragmentSrc));
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
        m_CameraRotation -= m_CameraRotationSpeed * ts;
    else
#ifndef __EMSCRIPTEN__
    if(Lucky::Input::IsKeyPressed(LK_KEY_Q))
#else
    if(Lucky::Input::IsKeyPressed(LK_KEY_A))
#endif
        m_CameraRotation += m_CameraRotationSpeed * ts;

    if(Lucky::Input::IsKeyPressed(LK_KEY_SPACE))
    {
        m_CameraPosition = glm::vec3(0.0f);
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

    m_Camera.SetPosition(m_CameraPosition);
    m_Camera.SetRotation(m_CameraRotation);

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
    
    glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), glm::radians(m_triangleRotation), glm::vec3(0.0f, 0.0f, 1.0f));
    glm::mat4 transform = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f)) * rotation;
    Lucky::Renderer::Submit(m_Shader, m_VertexArray, transform);

    Lucky::Renderer::EndScene();
}

void ExampleLayer::OnImGuiRender()
{
    ImGui::Begin("Camera");
    ImGui::Text("Position: %.1f, %.1f", m_CameraPosition.x, m_CameraPosition.y);
    ImGui::Text("Rotation: %.1f", -m_CameraRotation); //Inverse because rotation is in counterclockwise degrees
    ImGui::Separator();
    ImGui::ColorEdit3("Square color", glm::value_ptr(m_SquareColor));
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
