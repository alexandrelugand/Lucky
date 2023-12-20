#include "LuckyPch.h"
#include "Application.h"

#include "Lucky/Core/Timestep.h"
#include "Lucky/Core/Renderer/Renderer.h"

namespace Lucky
{
    #ifdef __EMSCRIPTEN__
        std::unique_ptr<Window> Application::m_Window;
        ImGuiLayer* Application::m_ImGuiLayer;
        LayerStack Application::m_LayerStack;
        float Application::m_LastFrameTime = 0.0f;
        bool Application::m_Minimized = false;
    #endif 

    Application* Application::s_Instance;

    Application::Application(const WindowProps &props)
    {
        LK_CORE_ASSERT(s_Instance == nullptr, "Application instance alreay set");
        s_Instance = this;
        m_Minimized = false;
        
        Log::Init();
        m_Window = std::unique_ptr<Window>(Window::Create(props));
        m_Window->SetEventCallback(BIND_EVENT_FN(Application::OnEvent));

        Renderer::Init();

        m_ImGuiLayer = new ImGuiLayer();
        PushOverlay(m_ImGuiLayer);
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
        float time = (float)glfwGetTime();
        Timestep timestep = time - m_LastFrameTime;
        m_LastFrameTime = time;

        if(!m_Minimized)
        {
            for(auto layer : m_LayerStack)
                layer->OnUpdate(timestep);
        }

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
        dispatcher.Dispatch<WindowResizeEvent>(BIND_EVENT_FN(Application::OnWindowResize));

        for(auto it = m_LayerStack.end(); it != m_LayerStack.begin();)
        {
            (*--it)->OnEvent(e);
            if(e.Handled())
                break;
        }
    }

    bool Application::OnWindowClose(WindowCloseEvent& e)
    {
        m_Running = false;
        return true;
    }

    bool Application::OnWindowResize(WindowResizeEvent& e)
    {
        if(e.GetWidth() == 0 || e.GetHeight() == 0)
        {
            m_Minimized = true;
            return false;
        }

        m_Minimized = false;
        Renderer::OnWindowResize(e.GetWidth(), e.GetHeight());
        return false;
    }
}
