#pragma once
//#include <SDL2/SDL.h>
#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#define GLFW_INCLUDE_ES3
#endif

#include <string>
#include <memory>
#include <GLFW/glfw3.h>

#include "Window.h"

namespace Lucky
{
    class Application
    {
    public:
        Application(const std::string& title);
        virtual ~Application();

        void Run();

    private:
#ifdef __EMSCRIPTEN__
        static std::unique_ptr<Window> m_Window;
#else
        //GLFWwindow* m_Window;
        std::unique_ptr<Window> m_Window;
#endif 
        static void ErrorCallback(int error, const char* description);
        static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

#ifdef __EMSCRIPTEN__
        static void RenderFrame();
#else
        void RenderFrame();
#endif        

    };

    //To be defined in client
    Application* CreateApplication();
}