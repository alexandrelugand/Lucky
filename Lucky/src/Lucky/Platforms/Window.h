#pragma once

#include "Lucky/Application/Window.h"
#include "Lucky/Renderer/Context.h"

namespace Lucky
{
    class WindowsWindow : public Window
    {
    public:
        WindowsWindow(const WindowProps& props);
        virtual ~WindowsWindow();

        void OnUpdate() override;

        uint32_t GetWidth() const override { return m_Data.Width; }
        uint32_t GetHeight() const override { return m_Data.Height; }
        bool IsVSync() const override { return m_Data.VSync; }
        void* GetNativeWindow() const override { return (void*)m_Window; }

        //Window attributes
        void SetEventCallback(const EventCallbackFn& fn) override { m_Data.EventCallback = fn; }
        void SetVSync(bool enabled) override;

    private:
        void Init(const WindowProps& props);
        void Shutdown();

        GLFWwindow* m_Window;
        Context* m_Context;

        struct WindowData
        {
            std::string Title;
            uint32_t Width, Height;
            bool VSync;

            EventCallbackFn EventCallback;
        };

        WindowData m_Data;
    };
}
