#pragma once

#include "Window.h"
#include "Lucky/Core/Events/ApplicationEvent.h"
#include "Lucky/Core/ImGui/ImGuiLayer.h"
#include "Lucky/Core/LayerStack.h"

#ifdef __EMSCRIPTEN__
    #define WASM_API static
#else
    #define WASM_API
#endif

namespace Lucky
{
    class Application
    {
    public:
        Application(const WindowProps& props);
        virtual ~Application();

        static inline Application& Get() { return *s_Instance; }

        void Run();
        void PushLayer(Layer* layer);
        void PushOverlay(Layer* overlay);
        inline Window& GetWindow() const { return *m_Window; }

    private:
        static Application* s_Instance;
        WASM_API std::unique_ptr<Window> m_Window;
        WASM_API ImGuiLayer* m_ImGuiLayer;
        WASM_API LayerStack m_LayerStack;
        WASM_API float m_LastFrameTime;
        bool m_Running;

        WASM_API void RenderFrame();
        void OnEvent(Event& e);
        bool OnWindowClose(WindowCloseEvent& event);
    };

    //To be defined in client
    Application* CreateApplication(const WindowProps& props);
}