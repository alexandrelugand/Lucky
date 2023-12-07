#include <iostream>
#include <stdexcept>
#include <format>
#ifdef __EMSCRIPTEN__
#include <GLES3/gl3.h>
#else
#include <glad/glad.h>
#endif
#include <GLFW/glfw3.h>

#include "Application.h"
#include "Lucky/Core/Core.h"
#include "Lucky/Core/Log.h"

namespace Lucky
{
    #ifdef __EMSCRIPTEN__
        std::unique_ptr<Window> Application::m_Window;
        LayerStack Application::m_LayerStack;
    #endif 

    Application* Application::s_Instance;

    Application::Application(const std::string& title)
        : m_Running(true)
    {
        CORE_ASSERT(s_Instance == nullptr, "Application instance alreay set");
        s_Instance = this;
        
        Log::Init();
        m_Window = std::unique_ptr<Window>(Window::Create());
        m_Window->SetEventCallback(BIND_EVENT_FN(Application::OnEvent));

        unsigned int id;
        glGenVertexArrays(1, &id);
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
        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        for(auto layer : m_LayerStack)
            layer->OnUpdate();

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