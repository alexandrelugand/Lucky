#include "LuckyEditorPch.h"
#include "EditorLayer.h"

namespace Lucky
{
	EditorLayer::EditorLayer()
		: Layer("EditorLayer")
	{
		auto &window = Application::Get().GetWindow();

		CameraSettings settings;
		settings.AspectRatio = (float)window.GetWidth() / (float)window.GetHeight();
		settings.EnableRotation = true;
		settings.TranslationSpeed = 10.0f;
		settings.RotationSpeed = 90.0f;
		settings.ZoomSpeed = 0.5f;
		settings.ZoomLevel = 10.0f;

		m_CameraController = CameraController::Create(CameraType::Orthographic, settings);
	}

	void EditorLayer::OnAttach()
	{
		LK_PROFILE_FUNCTION();

		auto &window = Application::Get().GetWindow();
		FramebufferSpecification fbSpec;
		fbSpec.Width = window.GetWidth();
		fbSpec.Height = window.GetHeight();
		m_Framebuffer = Framebuffer::Create(fbSpec);

#ifdef __EMSCRIPTEN__ 
		LK_TRACE("Load default layout");
		ImGui::LoadIniSettingsFromDisk("assets/layout/imgui.ini");
#endif	
	}

	void EditorLayer::OnDetach()
	{
		LK_PROFILE_FUNCTION();
	}

	void EditorLayer::OnUpdate(Timestep ts)
	{
		LK_PROFILE_FUNCTION();

		if(m_ViewportFocused)
			m_CameraController->OnUpdate(ts);

		Renderer2D::ResetStats();
		m_Framebuffer->Bind();

		// Prepare scene
		{
			LK_PROFILE_SCOPE("void EditorLayer::OnUpdate() - Prepare");
			RenderCommand::SetClearColor({0.1f, 0.1f, 0.1f, 1.0f});
			RenderCommand::Clear();
		}

		// Example scene
		{
			LK_PROFILE_SCOPE("void EditorLayer::OnUpdate() - Example scene");
			Renderer2D::BeginScene(m_CameraController);

			Renderer2D::DrawQuad({-1.0f, 0.0f}, {0.8f, 0.8f}, {0.8f, 0.2f, 0.3f, 1.0f});
			Renderer2D::DrawQuad({0.5f, -0.5f}, {0.5f, 0.75f}, {0.2f, 0.3f, 0.8f, 1.0f});

			for (float y = -5.0f; y < 5.0f; y += 0.5f)
			{
				for (float x = -5.0f; x < 5.0f; x += 0.5f)
				{
					glm::vec4 color = {(x + 5.0f) / 10.0f, 0.4f, (y + 5.0f) / 10.0f, 0.7f};
					Renderer2D::DrawQuad({x, y, -0.4f}, {0.45f, 0.45f}, color);
				}
			}

			Renderer2D::EndScene();
		}

		m_Framebuffer->Unbind();
	}

	void EditorLayer::OnImGuiRender()
	{
		static uint32_t loadLayout = 0;
		bool save = false, load = false;
		static ImGuiDockNodeFlags dockSpace_flags = ImGuiDockNodeFlags_None;

		// We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
		// because it would be confusing to have two docking targets within each others.
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
		const ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(viewport->WorkPos);
		ImGui::SetNextWindowSize(viewport->WorkSize);
		ImGui::SetNextWindowViewport(viewport->ID);
	
		window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
		window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

		// When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background
		// and handle the pass-thru hole, so we ask Begin() to not render a background.
		if (dockSpace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
			window_flags |= ImGuiWindowFlags_NoBackground;

		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);

		ImGui::Begin("Lucky Editor", nullptr, window_flags);

		// Submit the DockSpace
		ImGuiIO& io = ImGui::GetIO();
		if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
		{
			ImGuiID dockspace_id = ImGui::GetID("EditorDockSpace");
			ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockSpace_flags);
		}

		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("Load layout", NULL, false, true))
					load = true;

				if (ImGui::MenuItem("Save layout", NULL, false, true))
					save = true;

#ifndef __EMSCRIPTEN__

				ImGui::Separator();

				if (ImGui::MenuItem("Exit", NULL, false, true))
					Application::Get().Close();
#endif
				ImGui::EndMenu();
			}

			ImGui::EndMenuBar();
		}

		m_CameraController->OnImGuiRender();

		auto stats = Renderer2D::GetStats();
		ImGui::Begin("Stats");
		ImGui::Text("Draw calls: %d", stats.DrawCalls),
		ImGui::Text("Quads: %d", stats.QuadCount),
		ImGui::Text("Vertices: %d", stats.GetTotalVertexCount()),
		ImGui::Text("Indices: %d", stats.GetTotalIndexCount()),
		ImGui::End();

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("Viewport");

		m_ViewportFocused = ImGui::IsWindowFocused();
		m_ViewportHovered = ImGui::IsWindowHovered();
		Application::Get().GetImGuiLayer()->BlockEvents(!m_ViewportFocused || !m_ViewportHovered);

		ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
		if(m_ViewportSize != *((glm::vec2*)&viewportPanelSize) && viewportPanelSize.x > 0 && viewportPanelSize.y > 0)
		{
			m_Framebuffer->Resize((uint32_t)viewportPanelSize.x, (uint32_t)viewportPanelSize.y);
			m_ViewportSize = { viewportPanelSize.x, viewportPanelSize.y };
			m_CameraController->OnResize(m_ViewportSize.x, m_ViewportSize.y);
		}
		ImGui::Image((ImTextureID)(intptr_t)m_Framebuffer->GetColorAttachmentRendererId(), ImVec2{ m_ViewportSize.x, m_ViewportSize.y }, ImVec2 { 0, 1 }, ImVec2 { 1, 0 });
		ImGui::End();
		ImGui::PopStyleVar();

		ImGui::End();
		ImGui::PopStyleVar(2);
		
		if(loadLayout < 10)
		{
			Application::Get().GetImGuiLayer()->Load();
			loadLayout++;
		}

		if(save)
			Application::Get().GetImGuiLayer()->Save();

		if(load)
			Application::Get().GetImGuiLayer()->Load();
	}

	void EditorLayer::OnEvent(Event &event)
	{
		m_CameraController->OnEvent(event);
	}
} // namespace Lucky
