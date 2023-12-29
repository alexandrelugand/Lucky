#include "LuckyPch.h"
#include "ImGuiLayer.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "Lucky/Application/Application.h"

namespace Lucky
{
	ImGuiLayer::ImGuiLayer()
		: Layer("ImGui")
	{
	}

	ImGuiLayer::~ImGuiLayer()
	{
		ImGuiLayer::OnDetach();
	}

	void ImGuiLayer::OnAttach()
	{
		LK_PROFILE_FUNCTION();
		LK_CORE_TRACE("ImGui create context");
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();

		ImGuiIO &io = ImGui::GetIO();
		//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;	  // Enable Docking
#ifndef __EMSCRIPTEN__
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable; // Enable Viewports
#endif
		io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;
		io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;

		ImGui::StyleColorsDark();

		ImGuiStyle &style = ImGui::GetStyle();
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			style.WindowRounding = 0.0f;
			style.Colors[ImGuiCol_WindowBg].w = 1.0f;
		}

		Application &app = Application::Get();
		Window &win = app.GetWindow();
		GLFWwindow *glWin = (GLFWwindow *)win.GetNativeWindow();
		ImGui_ImplGlfw_InitForOpenGL(glWin, true);

#ifdef __EMSCRIPTEN__
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
		glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
		ImGui_ImplOpenGL3_Init("#version 300 es");
		io.IniFilename = "/data/imgui.ini";
#else
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
		ImGui_ImplOpenGL3_Init("#version 410");
#endif
	}

	void ImGuiLayer::OnDetach()
	{
		LK_PROFILE_FUNCTION();
#ifdef __EMSCRIPTEN__
		LK_CORE_INFO("Save layout on detach");
		ImGuiIO &io = ImGui::GetIO();
		ImGui::SaveIniSettingsToDisk(io.IniFilename);
		EM_ASM(
			FS.syncfs(function (err) {
				if(err != undefined)
					console.log("Save layout on detach failed. " + err);
			});
		);
#endif
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}

	void ImGuiLayer::Begin()
	{
		LK_PROFILE_FUNCTION();
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
	}

	void ImGuiLayer::End()
	{
		LK_PROFILE_FUNCTION();
		ImGuiIO &io = ImGui::GetIO();
		Application &app = Application::Get();
		Window &win = app.GetWindow();
		io.DisplaySize = ImVec2((float)win.GetWidth(), (float)win.GetHeight());

		// Rendering
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			GLFWwindow *backup_current_context = glfwGetCurrentContext();
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			glfwMakeContextCurrent(backup_current_context);
		}
	}

	void ImGuiLayer::OnImGuiRender()
	{
#ifdef IMGUI_DEMO
		static bool show = true;
		ImGui::ShowDemoWindow(&show);
#endif		
	}

	void ImGuiLayer::OnEvent(Event& event)
	{
		if(m_BlockEvents)
		{
			ImGuiIO& io = ImGui::GetIO();
			event.Handled |= event.IsInCategory(EventCategoryMouse) & io.WantCaptureMouse;
			event.Handled |= event.IsInCategory(EventCategoryKeyboard) & io.WantCaptureKeyboard;
		}
	}

	void ImGuiLayer::Load()
	{
		LK_CORE_TRACE("Load layout");
		ImGuiIO &io = ImGui::GetIO();
#ifdef __EMSCRIPTEN__
		ImGui::LoadIniSettingsFromDisk(io.IniFilename);
#else		
		ImGui::LoadIniSettingsFromDisk(io.IniFilename);
#endif		
	}

	void ImGuiLayer::Save()
	{
		LK_CORE_TRACE("Save layout");
		ImGuiIO &io = ImGui::GetIO();
#ifdef __EMSCRIPTEN__
		ImGui::SaveIniSettingsToDisk(io.IniFilename);
		EM_ASM(
			FS.syncfs(function (err) {
				if(err != undefined)
					console.log("Save layout failed. " + err);
			});
		);
#else		
		ImGui::SaveIniSettingsToDisk(io.IniFilename);
#endif	
	}
} // namespace Lucky
