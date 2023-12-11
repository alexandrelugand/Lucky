#pragma once
#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#define GLFW_INCLUDE_ES3
#endif

#include <string>
#include <memory>
#include <GLFW/glfw3.h>

#include "Window.h"
#include "Lucky/Core/Events/ApplicationEvent.h"
#include "Lucky/Core/ImGui/ImGuiLayer.h"
#include "Lucky/Core/LayerStack.h"
#include "Lucky/Core/Renderer/VertexArray.h"
#include "Lucky/Core/Renderer/VertexBuffer.h"
#include "Lucky/Core/Renderer/IndexBuffer.h"
#include "Lucky/Core/Renderer/Shader.h"

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
        Application(const std::string& title);
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
        WASM_API std::shared_ptr<VertexArray> m_VertexArray;
        WASM_API std::shared_ptr<VertexArray> m_squareVA;
        WASM_API std::shared_ptr<Shader> m_Shader;
        WASM_API std::shared_ptr<Shader> m_BlueShader;
        bool m_Running;

        WASM_API void RenderFrame();
        void OnEvent(Event& e);
        bool OnWindowClose(WindowCloseEvent& event);
    };

    //To be defined in client
    Application* CreateApplication();
}