#include "LuckyPch.h"
#include "ImGuiLayer.h"

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>	
#include "Lucky/Application/Application.h"
#include "ImGuizmo.h"

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

		io.Fonts->AddFontFromFileTTF("assets/fonts/opensans/OpenSans-Bold.ttf", 18.0f);
		io.FontDefault = io.Fonts->AddFontFromFileTTF("assets/fonts/opensans/OpenSans-Regular.ttf", 18.0f);

		ImGui::StyleColorsDark();

		ImGuiStyle &style = ImGui::GetStyle();
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			style.WindowRounding = 0.0f;
			style.Colors[ImGuiCol_WindowBg].w = 1.0f;
		}

		SetDartThemeColors();

		Application &app = Application::Get();
		Window &win = app.GetWindow();
		GLFWwindow *glWin = (GLFWwindow *)win.GetNativeWindow();
		ImGui_ImplGlfw_InitForOpenGL(glWin, true);

#ifdef __EMSCRIPTEN__
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
		glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
		ImGui_ImplOpenGL3_Init("#version 300 es");
		io.IniFilename = STRCAT(ASSETS, "/layout/imgui.ini").c_str();
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
		ImGuizmo::BeginFrame();
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

	void ImGuiLayer::SetDartThemeColors()
	{
		auto& colors = ImGui::GetStyle().Colors;

		colors[ImGuiCol_WindowBg] = ImVec4{ 0.1f, 0.105f, 0.11f, 1.0f };

		// Headers
		colors[ImGuiCol_Header] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };
		colors[ImGuiCol_HeaderHovered] = ImVec4{ 0.3f, 0.305f, 0.31f, 1.0f };
		colors[ImGuiCol_HeaderActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };

		// Buttons
		colors[ImGuiCol_Button] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };
		colors[ImGuiCol_ButtonHovered] = ImVec4{ 0.3f, 0.305f, 0.31f, 1.0f };
		colors[ImGuiCol_ButtonActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };

		// Frame BG
		colors[ImGuiCol_FrameBg] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };
		colors[ImGuiCol_FrameBgHovered] = ImVec4{ 0.3f, 0.305f, 0.31f, 1.0f };
		colors[ImGuiCol_FrameBgActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };

		// Tabs
		colors[ImGuiCol_Tab] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
		colors[ImGuiCol_TabHovered] = ImVec4{ 0.38f, 0.3805f, 0.381f, 1.0f };
		colors[ImGuiCol_TabActive] = ImVec4{ 0.28f, 0.2805f, 0.281f, 1.0f };
		colors[ImGuiCol_TabUnfocused] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
		colors[ImGuiCol_TabUnfocusedActive] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };

		// Title BG
		colors[ImGuiCol_TitleBg] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
		colors[ImGuiCol_TitleBgActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
		colors[ImGuiCol_TitleBgCollapsed] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
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
}
