#include "LuckyPch.h"
#include "WindowsWindow.h"

#include "Lucky/Platforms/OpenGL/OpenGLContext.h"
#include "Lucky/Core/Events/Event.h"
#include "Lucky/Core/Events/KeyEvent.h"
#include "Lucky/Core/Events/MouseEvent.h"
#include "Lucky/Core/Events/ApplicationEvent.h"

namespace Lucky
{
    static bool s_GLFWInitialized = false;

    Scope<Window> Window::Create(const WindowProps &props)
    {
        return CreateScope<WindowsWindow>(props);
    }

    WindowsWindow::WindowsWindow(const WindowProps &props)
    {
		LK_PROFILE_FUNCTION();
		Init(props);
    }

    WindowsWindow::~WindowsWindow()
    {
		LK_PROFILE_FUNCTION();
		Shutdown();
    }

    void WindowsWindow::OnUpdate()
    {
		LK_PROFILE_FUNCTION();
        glfwPollEvents();
        m_Context->SwapBuffers();
    }

    void WindowsWindow::SetVSync(bool enabled)
    {
		LK_PROFILE_FUNCTION();
		if (enabled)
            glfwSwapInterval(1);
        else
            glfwSwapInterval(0);

        m_Data.VSync = enabled;
    }

    void WindowsWindow::Init(const WindowProps &props)
    {
		LK_PROFILE_FUNCTION();
		m_Data.Title = props.Title;
        m_Data.Width = props.Width;
        m_Data.Height = props.Height;

        LK_CORE_INFO("Creating window {0} ({1}, {2})", props.Title, props.Width, props.Height);

        if (!s_GLFWInitialized)
        {
            bool success = glfwInit() != 0;

            glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
            glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

            LK_CORE_ASSERT(success, "Could not initialize GLFW!");
            glfwSetErrorCallback([](int error_code, const char* description)
            {
                LK_CORE_ERROR("GLFW Error ({0}): {1}", error_code, description);
            });
            s_GLFWInitialized = success;
        }

        m_Window = glfwCreateWindow((int)m_Data.Width, (int)m_Data.Height, m_Data.Title.c_str(), nullptr, nullptr);
        LK_CORE_ASSERT(m_Window != nullptr, "Could not create window!");

#ifndef __EMSCRIPTEN__
        int max_width  = GetSystemMetrics(SM_CXSCREEN);
        int max_hieght = GetSystemMetrics(SM_CYSCREEN);
        glfwSetWindowMonitor(m_Window, NULL, (max_width/2)-(m_Data.Width/2), (max_hieght/2) - (m_Data.Height/2), m_Data.Width, m_Data.Height, GLFW_DONT_CARE);
#endif
        m_Context = new OpenGLContext(m_Window);
        m_Context->Init();

        glfwSetWindowUserPointer(m_Window, &m_Data);
        SetVSync(props.VSync);

        // Setup GLFW callbacks
        glfwSetKeyCallback(m_Window, [](GLFWwindow *window, int key, int scancode, int action, int mods)
        {
            WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
            switch(action)
            {
            case GLFW_PRESS:
                {
                    KeyPressedEvent event((KeyCode)key, 0);
                    data.EventCallback(event);
                    if(key == GLFW_KEY_ESCAPE)
                    {
                        WindowCloseEvent event;
                        data.EventCallback(event); 
                    }
                    break;
                }
            case GLFW_RELEASE:
                {
                    KeyReleasedEvent event((KeyCode)key);
                    data.EventCallback(event);
                    break;
                }
            case GLFW_REPEAT:
                {
                    KeyPressedEvent event((KeyCode)key, 1);
                    data.EventCallback(event);
                    break;
                }
            } 
        });

        glfwSetCharCallback(m_Window, [](GLFWwindow *window, unsigned int character)
        {
            WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
            KeyTypedEvent event((KeyCode)character);
            data.EventCallback(event);
        });

        glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* window, int button, int action, int mods)
        {
            WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
            switch(action)
            {
            case GLFW_PRESS:
                {
                    MouseButtonPressedEvent event((MouseButton)button);
                    data.EventCallback(event);
                    break;
                }
            case GLFW_RELEASE:
                {
                    MouseButtonReleasedEvent event((MouseButton)button);
                    data.EventCallback(event);
                    break;
                }
            } 
        });

        glfwSetScrollCallback(m_Window, [](GLFWwindow* window, double xoffset, double yoffset)
        {
            WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
            MouseScrolledEvent event((float)xoffset, (float)yoffset);
            data.EventCallback(event);
        });

        glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double xpos, double ypos)
        {
            WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
            MouseMovedEvent event((float)xpos, (float)ypos);
            data.EventCallback(event);
        });

        glfwSetWindowSizeCallback(m_Window, [](GLFWwindow *window, int width, int height)
        {
            WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
            data.Width = width;
            data.Height = height;

            WindowResizeEvent event(width, height);
            data.EventCallback(event);
        });

        glfwSetWindowCloseCallback(m_Window, [](GLFWwindow *window)
        {
            WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
            WindowCloseEvent event;
            data.EventCallback(event); 
        });
    }

    void WindowsWindow::Shutdown()
    {
		LK_PROFILE_FUNCTION();
		glfwDestroyWindow(m_Window);
        glfwTerminate();
    }
} // namespace Lucky
