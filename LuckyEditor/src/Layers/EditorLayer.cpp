#include "LuckyEditorPch.h"
#include "EditorLayer.h"

#include "Lucky/Platforms/Platform.h"

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

		FramebufferSpecification fbSpec;
		fbSpec.Width = window.GetWidth();
		fbSpec.Height = window.GetHeight();
		m_Framebuffer = Framebuffer::Create(fbSpec);

#if 1
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

		auto cameraA = m_ActiveScene->CreateEntity("Camera A");
		cameraA.AddComponent<CameraComponent>(settings);
		cameraA.AddComponent<NativeScriptComponent>().Bind<CameraController>();

		auto cameraB = m_ActiveScene->CreateEntity("Camera B");
		cameraB.AddComponent<CameraComponent>(settings).Primary = false;

		auto greenSquare = m_ActiveScene->CreateEntity("Green Square");
		greenSquare.AddComponent<SpriteRendererComponent>(glm::vec4{ 0.0f, 1.0f, 0.0f, 1.0f });
		greenSquare.GetComponent<TransformComponent>().Translation = glm::vec3{ -2.0f, 0.0f, 0.0f };

		auto redSquare = m_ActiveScene->CreateEntity("Red Square");
		redSquare.AddComponent<SpriteRendererComponent>(glm::vec4{ 1.0f, 0.0f, 0.0f, 1.0f });
		redSquare.GetComponent<TransformComponent>().Translation = glm::vec3{ 2.0f, 0.0f, -10.0f };
#endif
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
		static uint32_t tryLoadLayout = 0;
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
				// Don't bind shortcuts on Ctrl because we can't override Chrome shortcuts
				if (ImGui::MenuItem("New", "Shift+N"))
					m_NewScene = true;				

				if (ImGui::MenuItem("Open...", "Shift+O"))
					m_OpenScene = true;

				if (ImGui::MenuItem("Save...", "Shift+S"))
					m_SaveScene = true;

				ImGui::Separator();

				if (ImGui::MenuItem("Load layout", "Alt+L"))
					m_LoadLayout = true;

				if (ImGui::MenuItem("Save layout", "Alt+S"))
					m_SaveLayout = true;

#ifndef __EMSCRIPTEN__

				ImGui::Separator();

				if (ImGui::MenuItem("Exit", "Esc"))
					Application::Get().Close();
#endif
				ImGui::EndMenu();
			}

			ImGui::EndMenuBar();
		}

		m_SceneHierarchyPanel.OnImGuiRenderer();

		auto stats = Renderer2D::GetStats();
		ImGui::Begin("Stats");
		ImGui::Text("Draw calls: %d", stats.DrawCalls);
		ImGui::Text("Quads: %d", stats.QuadCount);
		ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
		ImGui::Text("Indices: %d", stats.GetTotalIndexCount());

		ImGui::Separator();
		static bool open = false;
		if(ImGui::Button("Show Demo"))
			open = true;

		if(open)			
			ImGui::ShowDemoWindow(&open);

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
		
		if(tryLoadLayout < 10)
		{
			Application::Get().GetImGuiLayer()->Load();
			tryLoadLayout++;
		}

		if(m_NewScene)
			NewScene();

		if(m_OpenScene)
			OpenScene();

		if(m_SaveScene)
			SaveScene();

		if (m_LoadLayout)
			LoadLayout();

		if (m_SaveLayout)
			SaveLayout();
	}

	void EditorLayer::OnEvent(Event &event)
	{
		m_ActiveScene->OnEvent(event);
		EventDispatcher dispatcher(event);
		dispatcher.Dispatch<KeyPressedEvent>(BIND_EVENT_FN(EditorLayer::OnKeyPressed));
	}

	void EditorLayer::NewScene()
	{
		m_ActiveScene = CreateRef<Scene>();
		m_ActiveScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
		m_SceneHierarchyPanel.SetContext(m_ActiveScene);
		m_NewScene = false;
	}

	void EditorLayer::OpenScene()
	{
		std::string filePath = Platform::OpenFile("Lucky Scene (*.lucky)\0*.lucky\0All files\0*.*\0\0", STRCAT(ASSETS, "/scenes"));
		if (!filePath.empty())
		{
			if (filePath != "##Cancel")
			{
				m_ActiveScene = CreateRef<Scene>();
				m_ActiveScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
				m_SceneHierarchyPanel.SetContext(m_ActiveScene);

				SceneSerializer serializer(m_ActiveScene);
				serializer.Deserialize(filePath);
			}
			m_OpenScene = false;
		}
	}

	void EditorLayer::SaveScene()
	{
		std::string filePath = Platform::SaveFile("Lucky Scene (*.lucky)\0*.lucky;\0\0", STRCAT(ASSETS, "/scenes"));
		if (!filePath.empty())
		{
			if (filePath != "##Cancel")
			{
				SceneSerializer serializer(m_ActiveScene);
				serializer.Serialize(filePath);
			}
			m_SaveScene = false;
		}
	}

	void EditorLayer::LoadLayout()
	{
		Application::Get().GetImGuiLayer()->Load();
		m_LoadLayout = false;
	}

	void EditorLayer::SaveLayout()
	{
		Application::Get().GetImGuiLayer()->Save();
		m_SaveLayout = false;
	}

	bool EditorLayer::OnKeyPressed(KeyPressedEvent& e)
	{
		if (e.GetRepeatCount() > 0)
			return false;

		bool alt = Input::IsKeyPressed(KeyCode::LeftAlt) || Input::IsKeyPressed(KeyCode::RightAlt);
		bool shift = Input::IsKeyPressed(KeyCode::LeftShift) || Input::IsKeyPressed(KeyCode::RightShift);

		// Shortcuts
		switch (e.GetKeyCode())
		{
		case KeyCode::N:
			{
				if (shift)
					m_NewScene = true;
				break;
			}

		case KeyCode::S:
			{
				if (shift)
					m_SaveScene = true;
				if (alt)
					m_SaveLayout = true;
				break;
			}

		case KeyCode::O:
			{
				if (shift)
					m_OpenScene = true;
				break;
			}

		case KeyCode::L:
			{
				if (alt)
					m_LoadLayout = true;
				break;
			}

		default:
			break;
		}

		return false;
	}
} // namespace Lucky
