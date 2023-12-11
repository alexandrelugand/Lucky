#include "LuckyPch.h"

#include <iostream>
#include <stdexcept>
#include <format>
#ifdef __EMSCRIPTEN__
#include <GLES3/gl3.h>
#else
#include <glad/glad.h>
#endif
#include <GLFW/glfw3.h>
#include <memory>

namespace Lucky
{
    #ifdef __EMSCRIPTEN__
        std::unique_ptr<Window> Application::m_Window;
        ImGuiLayer* Application::m_ImGuiLayer;
        LayerStack Application::m_LayerStack;
        std::shared_ptr<VertexArray> Application::m_VertexArray;
        std::shared_ptr<VertexArray> Application::m_squareVA;
        std::shared_ptr<Shader> Application::m_Shader;
        std::shared_ptr<Shader> Application::m_BlueShader;
    #endif 

    Application* Application::s_Instance;

    Application::Application(const std::string& title)
        : m_Running(true)
    {
        LK_CORE_ASSERT(s_Instance == nullptr, "Application instance alreay set");
        s_Instance = this;
        
        Log::Init();
        m_Window = std::unique_ptr<Window>(Window::Create());
        m_Window->SetEventCallback(BIND_EVENT_FN(Application::OnEvent));
        m_ImGuiLayer = new ImGuiLayer();
        PushOverlay(m_ImGuiLayer);

        m_VertexArray.reset(VertexArray::Create());
        m_VertexArray->Bind();

        float vertices[3 * 7] = 
        {
            -0.5f, -0.5f, 0.0f, 0.8f, 0.2f, 0.8f, 1.0f,
            0.5f, -0.5f, 0.0f, 0.2f, 0.3f, 0.8f, 1.0f,
            0.0f, 0.5f, 0.0f, 0.8f, 0.8f, 0.2f, 1.0f
        };
        std::shared_ptr<VertexBuffer> vertexBuffer;
        vertexBuffer.reset(VertexBuffer::Create(vertices, sizeof(vertices)));
        vertexBuffer->SetLayout({
            { ShaderDataType::Float3, "a_Position" },
            { ShaderDataType::Float4, "a_Color" }
        });
        m_VertexArray->AddVertexBuffer(vertexBuffer);

        unsigned int indices[3] = { 0, 1, 2 };
        std::shared_ptr<IndexBuffer> indexBuffer;
        indexBuffer.reset(IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
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

            void main()
            {
                v_Position = a_Position;
                v_Color = a_Color;
                gl_Position = vec4(a_Position, 1.0);
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

        m_Shader.reset(new Shader(vertexSrc, fragmentSrc));

        m_squareVA.reset(VertexArray::Create());
        m_squareVA->Bind();

        float squareVertices[4 * 3] = 
        {
            -0.75f, -0.75f, 0.0f,
            0.75f, -0.75f, 0.0f, 
            0.75f, 0.75f, 0.0f,
            -0.75f, 0.75f, 0.0f
        };
        std::shared_ptr<VertexBuffer> squareVB;
        squareVB.reset(VertexBuffer::Create(squareVertices, sizeof(squareVertices)));
        squareVB->SetLayout({
            { ShaderDataType::Float3, "a_Position" }
        });
        m_squareVA->AddVertexBuffer(squareVB);

        unsigned int squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
        std::shared_ptr<IndexBuffer> squareIB;
        squareIB.reset(IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));
        m_squareVA->SetIndexBuffer(squareIB);

#ifdef __EMSCRIPTEN__
        vertexSrc = "#version 300 es\n";
#else
        vertexSrc = "#version 410\n";
#endif
        vertexSrc += R"(
            layout(location = 0) in vec3 a_Position;
            out vec3 v_Position;

            void main()
            {
                v_Position = a_Position;
                gl_Position = vec4(a_Position, 1.0);
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

            void main()
            {
                color = vec4(0.2, 0.3, 0.8, 1.0);
            }
        )";

        m_BlueShader.reset(new Shader(vertexSrc, fragmentSrc));
    }

    Application::~Application()
    {
    }

    void Application::Run()
    {
        LK_CORE_INFO("Application is running");

        // Run the loop correctly for the target environment
#ifdef __EMSCRIPTEN__
        emscripten_set_main_loop(RenderFrame, 0, 1);
#else
        // Display the window until ESC is pressed
        while (m_Running) 
        {
            RenderFrame();
        }
#endif
    }

    void Application::PushLayer(Layer *layer)
    {
        m_LayerStack.PushLayer(layer);
        layer->OnAttach();
    }

    void Application::PushOverlay(Layer *overlay)
    {
        m_LayerStack.PushOverlay(overlay);
        overlay->OnAttach();
    }

    void Application::RenderFrame() 
    {
        // Background color
        RenderCommand::SetClearColor({0.1f, 0.1f, 0.1f, 1.0f});
        RenderCommand::Clear();

        Renderer::BeginScene();

        m_BlueShader->Bind();
        Renderer::Submit(m_squareVA);

        m_Shader->Bind();
        Renderer::Submit(m_VertexArray);

        Renderer::EndScene();

        m_ImGuiLayer->Begin();
        for(auto layer : m_LayerStack)
            layer->OnImGuiRender();
        m_ImGuiLayer->End();

        m_Window->OnUpdate();
    }

    void Application::OnEvent(Event &e)
    {
        EventDispatcher dispatcher(e);
        dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(Application::OnWindowClose));
        LK_CORE_TRACE("{0}", e);

        for(auto it = m_LayerStack.end(); it != m_LayerStack.begin();)
        {
            (*--it)->OnEvent(e);
            if(e.Handled())
                break;
        }
    }

    bool Application::OnWindowClose(WindowCloseEvent& event)
    {
        m_Running = false;
        return true;
    }
}