#include "LuckyEditorPch.h"
#include "EditorLayer.h"


namespace Lucky
{
	EditorLayer::EditorLayer()
		: Layer("EditorLayer")
	{
		m_ActiveScene = CreateRef<Scene>();
	}

	void EditorLayer::OnAttach()
	{
		LK_PROFILE_FUNCTION();

		auto& window = Application::Get().GetWindow();

		Camera::Settings settings;
		settings.ProjectionType = Camera::ProjectionType::Orthographic;
		settings.AspectRatio = (float)window.GetWidth() / (float)window.GetHeight();
		settings.EnableRotation = true;
		settings.TranslationSpeed = 10.0f;
		settings.RotationSpeed = 90.0f;
		settings.ZoomSpeed = 0.5f;
		settings.ZoomLevel = 1.0f;
		settings.OrthographicSize = 10.0f;
		settings.OrthographicNearClip = -100.0f;
		settings.OrthographicFarClip = 100.0f;

		FramebufferSpecification fbSpec;
		fbSpec.Width = window.GetWidth();
		fbSpec.Height = window.GetHeight();
		m_Framebuffer = Framebuffer::Create(fbSpec);

		class CameraController : public ScriptableEntity
		{
			const Camera::Settings c_Settings;

		public:
			CameraController(const Entity& entity)
				: ScriptableEntity(entity)
			{
			}

			void OnCreate() override
			{				
			}

			void OnDestroy() override
			{
			}

			void OnUpdate(Timestep ts) override
			{
				auto& tc = GetComponent<TransformComponent>();

#ifndef __EMSCRIPTEN__
				if (Input::IsKeyPressed(KeyCode::A))
#else
				if (Input::IsKeyPressed(KeyCode::Q))
#endif
				{
					tc.Translation.x -= cos(tc.Rotation.z) * c_Settings.TranslationSpeed * ts;
					tc.Translation.y -= sin(tc.Rotation.z) * c_Settings.TranslationSpeed * ts;
				}
				else
					if (Input::IsKeyPressed(KeyCode::D))
					{
						tc.Translation.x += cos(tc.Rotation.z) * c_Settings.TranslationSpeed * ts;
						tc.Translation.y += sin(tc.Rotation.z) * c_Settings.TranslationSpeed * ts;
					}

				if (Input::IsKeyPressed(KeyCode::S))
				{
					tc.Translation.x -= -sin(tc.Rotation.z) * c_Settings.TranslationSpeed * ts;
					tc.Translation.y -= cos(tc.Rotation.z) * c_Settings.TranslationSpeed * ts;
				}
				else
#ifndef __EMSCRIPTEN__
					if (Input::IsKeyPressed(KeyCode::W))
#else
					if (Input::IsKeyPressed(KeyCode::Z))
#endif
					{
						tc.Translation.x += -sin(tc.Rotation.z) * c_Settings.TranslationSpeed * ts;
						tc.Translation.y += cos(tc.Rotation.z) * c_Settings.TranslationSpeed * ts;
					}


					if (Input::IsKeyPressed(KeyCode::E))
						tc.Rotation.z -= c_Settings.RotationSpeed * ts;
					else
#ifndef __EMSCRIPTEN__
						if (Input::IsKeyPressed(KeyCode::Q))
#else
						if (Input::IsKeyPressed(KeyCode::A))
#endif
							tc.Rotation.z += c_Settings.RotationSpeed * ts;

				if (Input::IsKeyPressed(KeyCode::Space))
				{
					tc.Translation = glm::vec3(0.0f);
					tc.Rotation.z = 0.0f;
				}
			}

			void OnEvent(Event& e) override
			{
				EventDispatcher dispatcher(e);
				dispatcher.Dispatch<MouseScrolledEvent>(BIND_EVENT_FN(CameraController::OnMouseScrolled));
			}

			bool OnMouseScrolled(MouseScrolledEvent& e)
			{
				auto [tc, cc] = GetComponent<TransformComponent, CameraComponent>();
				auto& settings = cc.Camera.GetSettings();

				if (settings.ProjectionType == Camera::ProjectionType::Perspective)
				{
					tc.Translation.z -= e.GetYOffset() * settings.ZoomSpeed;
				}
				else
				{
					cc.Camera.SetOrthographicSize(cc.Camera.GetOrthographicSize() - e.GetYOffset() * settings.ZoomSpeed);
				}
				return false;
			}
		};

		m_CameraAEntity = m_ActiveScene->CreateEntity("Camera A");
		m_CameraAEntity.AddComponent<CameraComponent>(settings).Primary = true;
		m_CameraAEntity.AddComponent<NativeScriptComponent>().Bind<CameraController>();

		m_CameraBEntity = m_ActiveScene->CreateEntity("Camera B");
		m_CameraBEntity.AddComponent<CameraComponent>(settings);

		auto greenSquare = m_ActiveScene->CreateEntity("Green Square");
		greenSquare.AddComponent<SpriteRendererComponent>(glm::vec4{ 0.0f, 1.0f, 0.0f, 1.0f });
		greenSquare.GetComponent<TransformComponent>().Translation = glm::vec3{ -2.0f, 0.0f, 0.0f };

		auto redSquare = m_ActiveScene->CreateEntity("Red Square");
		redSquare.AddComponent<SpriteRendererComponent>(glm::vec4{ 1.0f, 0.0f, 0.0f, 1.0f });
		redSquare.GetComponent<TransformComponent>().Translation = glm::vec3{ 2.0f, 0.0f, -10.0f };

		m_SceneHierarchyPanel.SetContext(m_ActiveScene);

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

		auto fbSpec = m_Framebuffer->GetSpecification();
		if (m_ViewportSize.x > 0 && m_ViewportSize.y > 0 &&
			(fbSpec.Width != m_ViewportSize.x || fbSpec.Height != m_ViewportSize.y))
		{
			m_Framebuffer->Resize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
			m_ActiveScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
		}

		Renderer2D::ResetStats();
		m_Framebuffer->Bind();

		// Prepare scene
		RenderCommand::SetClearColor({0.1f, 0.1f, 0.1f, 1.0f});
		RenderCommand::Clear();

		m_ActiveScene->OnUpdate(ts);

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
		ImGuiStyle& style = ImGui::GetStyle();
		float minWinSizeX = style.WindowMinSize.x;
		style.WindowMinSize.x = 370.0f;
		if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
		{
			ImGuiID dockspace_id = ImGui::GetID("EditorDockSpace");
			ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockSpace_flags);
		}

		style.WindowMinSize.x = minWinSizeX;

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

		m_SceneHierarchyPanel.OnImGuiRenderer();

		ImGui::Begin("Settings");
		

		if(ImGui::Checkbox("Show Camera A", &m_CameraA))
		{
			m_CameraAEntity.GetComponent<CameraComponent>().Primary = m_CameraA;
			m_CameraBEntity.GetComponent<CameraComponent>().Primary = !m_CameraA;
		}

		ImGui::End();

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
		m_ViewportSize = *((glm::vec2*)&viewportPanelSize);
		
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
		m_ActiveScene->OnEvent(event);
	}
} // namespace Lucky
