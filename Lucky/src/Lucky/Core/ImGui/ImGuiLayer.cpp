#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#include <GLES3/gl3.h>
#else
#include <glad/glad.h>
#endif
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "ImGuiLayer.h"
#include "Lucky/Application/Application.h"
#include "Lucky/Application/Window.h"
// #include "Lucky/Platforms/OpenGL/imgui_impl_glfw.h"
// #include "Lucky/Platforms/OpenGL/imgui_impl_opengl3.h"
#include "Lucky/Core/Log.h"

namespace Lucky
{
    ImGuiLayer::ImGuiLayer()
        : Layer("ImGui")
    {
    }

    ImGuiLayer::~ImGuiLayer()
    {
    }

    void ImGuiLayer::OnAttach()
    {
        LK_CORE_INFO("ImGui create context");
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();

        ImGuiIO& io = ImGui::GetIO();(void)io;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;     // Enable Docking
        io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;
        io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;

        ImGui::StyleColorsDark();

        Application& app = Application::Get();
        Window& win = app.GetWindow();
        GLFWwindow* glWin = (GLFWwindow*)win.GetHandle();
        ImGui_ImplGlfw_InitForOpenGL(glWin, true);

#ifdef __EMSCRIPTEN__
        // For an Emscripten build we are disabling file-system access, so let's not attempt to do a fopen() of the imgui.ini file.
        // You may manually call LoadIniSettingsFromMemory() to load settings from your own storage.
        io.IniFilename = nullptr;
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
        glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
        ImGui_ImplOpenGL3_Init("#version 100");
#else
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
        ImGui_ImplOpenGL3_Init("#version 130");
#endif
    }

    void ImGuiLayer::OnDetach()
    {
    }

    void ImGuiLayer::OnUpdate()
    {
        LK_CORE_TRACE("ImGui OnUpdate");
        ImGuiIO& io = ImGui::GetIO();(void)io;
        Application& app = Application::Get();
        Window& win = app.GetWindow();
        GLFWwindow* glWin = (GLFWwindow*)win.GetHandle();
        //io.DisplaySize = ImVec2(win.GetWidth(), win.GetHeight());

        float time = (float)glfwGetTime();
        io.DeltaTime = m_Time > 0.0f ? (time - m_Time) : (1.0f / 60.0f);
        m_Time = time;

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        static bool show = true;
        ImGui::ShowDemoWindow(&show);

        ImGui::Render();
        int width = win.GetWidth();
        int height = win.GetHeight();
        glfwGetFramebufferSize(glWin, &width, &height);
        glViewport(0, 0, width, height);
        ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
        glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT);

        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }

    void ImGuiLayer::OnEvent(Event &event)
    {
    }

} // namespace Lucky
