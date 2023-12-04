#include "WindowsWindow.h"
#include "Lucky/Core/Log.h"

namespace Lucky
{
    static bool s_GLFWInitialized = false;

    Window* Window::Create(const WindowProps& props)
    {
        return new WindowsWindow(props);
    }

    WindowsWindow::WindowsWindow(const WindowProps& props) 
    {
        Init(props);
    }

    WindowsWindow::~WindowsWindow()
    {
        Shutdown();
    }

    void WindowsWindow::OnUpdate()
    {
         // Clear the window with the background color
        glClear(GL_COLOR_BUFFER_BIT);
         // Handle any events
        glfwPollEvents();
        // Flip the double buffer
        glfwSwapBuffers(m_Window);
    }

    void WindowsWindow::SetVSync(bool enabled)
    {
        if(enabled)
            glfwSwapInterval(1);
        else
            glfwSwapInterval(0);

         m_Data.VSync = enabled;
    }

     // Handle key presses
    void WindowsWindow::KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) 
    {
        if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
            glfwSetWindowShouldClose(window, GLFW_TRUE);
    }

    bool WindowsWindow::ShouldClose() const
    {
        return glfwWindowShouldClose(m_Window) != 0;
    }

    void WindowsWindow::Init(const WindowProps &props)
    {
        m_Data.Title = props.Title;
        m_Data.Width = props.Width;
        m_Data.Height = props.Height;

        LK_CORE_INFO("Creating window {0} ({1}, {2})", props.Title, props.Width, props.Height);
        if(!s_GLFWInitialized)
        {
            bool success = glfwInit() != 0;
            CORE_ASSERT(success, "Could not initialize GLFW!");
            s_GLFWInitialized = success;
        }

        m_Window = glfwCreateWindow((int) m_Data.Width, (int) m_Data.Height, m_Data.Title.c_str(), nullptr, nullptr);
        CORE_ASSERT(m_Window != nullptr, "Could not create window!");
        glfwMakeContextCurrent(m_Window);
        glfwSetWindowUserPointer(m_Window, &m_Data);
        SetVSync(true);

        // Setup the Key Press handler
        glfwSetKeyCallback(m_Window, KeyCallback);

         // Near magenta background
        glClearColor(1.0f, 0.0f, 1.0f, 0.0f);
    }

    void WindowsWindow::Shutdown()
    {
        glfwDestroyWindow(m_Window);
        glfwTerminate();
    }
} // namespace Lucky