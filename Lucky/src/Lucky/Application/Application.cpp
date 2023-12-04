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

    // // Handle GLFW Errors
    // void Application::ErrorCallback(int error, const char* description) 
    // {
    //     LK_CORE_ERROR("Error: {}", description);
    // }

    // // Handle key presses
    // void Application::KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) 
    // {
    //     if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    //         glfwSetWindowShouldClose(window, GLFW_TRUE);
    // }

    // Generate the frame data.
    void Application::RenderFrame() 
    {
        m_Window->OnUpdate();
        // // Clear the window with the background color
        // glClear(GL_COLOR_BUFFER_BIT);
        // // Flip the double buffer
        // glfwSwapBuffers(m_Window);
        // // Handle any events
        // glfwPollEvents();
    }

    Application::Application(const std::string& title)
    {
        Log::Init();
        m_Window = std::unique_ptr<Window>(Window::Create());

        // LK_CORE_INFO("Initializing video...");

        // // Setup the Error handler
        // glfwSetErrorCallback(ErrorCallback);

        // if (!glfwInit()) 
        //     throw std::runtime_error("GLFW Initialization failed");

        // glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        // glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

        // // Create the display window
        // m_Window = glfwCreateWindow(800, 600, "Demo", NULL, NULL);
        // if (!m_Window) 
        //     throw std::runtime_error("GLFW Window Creation Failed");

        // // Setup the Key Press handler
        // glfwSetKeyCallback(m_Window, KeyCallback);
        // // Select the window as the drawing destination
        // glfwMakeContextCurrent(m_Window);

        // // Near white background
        // glClearColor(1.0f, 0.0f, 1.0f, 0.0f);
    }

    Application::~Application()
    {
        // // Clean up
        // if(m_Window != nullptr)
        //     glfwDestroyWindow(m_Window);
        //glfwTerminate();
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

    // void Application::DrawRandomPixels()
    // {
    //     if (SDL_MUSTLOCK(s_Surface))
    //         SDL_LockSurface(s_Surface);

    //     Uint8 *pixels = (Uint8 *)s_Surface->pixels;

    //     for (int i = 0; i < 1048576; i++)
    //     {
    //         char randomByte = rand() % 255;
    //         pixels[i] = randomByte;
    //     }

    //     if (SDL_MUSTLOCK(s_Surface))
    //         SDL_UnlockSurface(s_Surface);

    //     SDL_Texture *screenTexture = SDL_CreateTextureFromSurface(s_Renderer, s_Surface);

    //     SDL_RenderClear(s_Renderer);
    //     SDL_RenderCopy(s_Renderer, screenTexture, NULL, NULL);
    //     SDL_RenderPresent(s_Renderer);

    //     SDL_DestroyTexture(screenTexture);
    // }

    // void Application::MainLoop()
    // {
    //     DrawRandomPixels();
    //     SDL_Delay(100);
    // }
}