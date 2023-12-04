#include <iostream>
#include <stdexcept>
#include <format>

#include "Application.h"
#include "Lucky/Core/Log.h"
#include "Lucky/Core/Events/ApplicationEvent.h"

namespace Lucky
{
    #ifdef __EMSCRIPTEN__
        std::unique_ptr<Window> Application::m_Window;
    #endif 

    // Generate the frame data.
    void Application::RenderFrame() 
    {
        m_Window->OnUpdate();
    }

    Application::Application(const std::string& title)
    {
        Log::Init();
        m_Window = std::unique_ptr<Window>(Window::Create());
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
        while (!m_Window->ShouldClose()) 
        {
            RenderFrame();
        }
#endif
    }
}