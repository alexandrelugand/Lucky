#pragma once
#include <GLFW/glfw3.h>

#include "Lucky/Application/Window.h"

namespace Lucky
{
    class WindowsWindow : public Window
    {
    public:
        WindowsWindow(const WindowProps& props);
        virtual ~WindowsWindow();

        void OnUpdate() override;

        inline unsigned int GetWidth() const override { return m_Data.Width; }
        inline unsigned int GetHeight() const override { return m_Data.Height; }
        inline bool IsVSync() const override { return m_Data.VSync; }
        inline void* GetHandle() const override { return (void*)m_Window; }

        //Window attributes
        inline void SetEventCallback(const EventCallbackFn& fn) override { m_Data.EventCallback = fn; }
        void SetVSync(bool enabled) override;

    private:
        void Init(const WindowProps& props);
        void Shutdown();

        GLFWwindow* m_Window;

        struct WindowData
        {
            std::string Title;
            unsigned int Width, Height;
            bool VSync;

            EventCallbackFn EventCallback;
        };

        WindowData m_Data;
    };
} // namespace Lucky