#pragma once

#include "Window.h"
#include "Lucky/Core/Assert.h"
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
    struct ApplicationCommandLineArgs
    {
		int Count = 0;
		char** Args = nullptr;

		const char* operator[](int index) const
		{
			LK_CORE_ASSERT(index < Count);
			return Args[index];
		}
    };
    
    class Application
    {
    public:
        Application(const WindowProps& props, const ApplicationCommandLineArgs& args = ApplicationCommandLineArgs());
        virtual ~Application();

        static Application& Get() { return *s_Instance; }

		void Run();
		void Close();

        void PushLayer(Layer* layer);
        void PushOverlay(Layer* overlay);
        Window& GetWindow() const { return *m_Window; }
		ImGuiLayer* GetImGuiLayer() const { return m_ImGuiLayer; }

		ApplicationCommandLineArgs GetCommandLineArgs() const { return m_CommandLineArgs; }

    private:
		ApplicationCommandLineArgs m_CommandLineArgs;
        static Application* s_Instance;
        WASM_API Scope<Window> m_Window;
        WASM_API ImGuiLayer* m_ImGuiLayer;
        WASM_API LayerStack m_LayerStack;
        WASM_API float m_LastFrameTime;
        WASM_API bool m_Minimized;
        bool m_Running = true;

        WASM_API void RenderFrame();
        void OnEvent(Event& e);
        bool OnWindowClose(WindowCloseEvent& event);
        bool OnWindowResize(WindowResizeEvent& event);
    };

    //To be defined in client
    Application* CreateApplication(const WindowProps& props, ApplicationCommandLineArgs args);
}
