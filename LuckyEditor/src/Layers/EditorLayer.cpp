#include "LuckyEditorPch.h"
#include "EditorLayer.h"

#include "Lucky/Platforms/Platform.h"
#include "Lucky/Math/Math.h"
#include "Scripts/CameraScript.h"

namespace Lucky
{
	EditorLayer::EditorLayer()
		: Layer("EditorLayer")
	{
	}

	void EditorLayer::OnAttach()
	{
		LK_PROFILE_FUNCTION();

		const auto commandLineArgs = Application::Get().GetCommandLineArgs();
		if(commandLineArgs.Count > 1)
			OpenScene(commandLineArgs[1]);
		else
			NewScene();

#ifdef __EMSCRIPTEN__ 
		LK_TRACE("Load default layout");
		ImGui::LoadIniSettingsFromDisk("assets/layout/imgui.ini");
#endif

		m_IconPlay = Texture2D::Create("resources/icons/PlayButton.png");
		m_IconStop = Texture2D::Create("resources/icons/StopButton.png");
	}

	void EditorLayer::OnDetach()
	{
		LK_PROFILE_FUNCTION();
	}

	void EditorLayer::OnUpdate(Timestep ts)
	{
		LK_PROFILE_FUNCTION();

		for (auto& pass : m_ActiveScene->GetPasses())
		{
			auto& fbSpec = pass.Framebuffer->GetSpecification();
			if (m_ViewportSize.x > 0 && m_ViewportSize.y > 0 &&
				(fbSpec.Width != m_ViewportSize.x || fbSpec.Height != m_ViewportSize.y))
			{
				pass.Framebuffer->Resize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
				m_EditorCamera.SetViewportSize(m_ViewportSize.x, m_ViewportSize.y);
				m_ActiveScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
				auto camera = m_ActiveScene->GetPrimaryCamera();
				if(camera)
				{
					auto& cameraComponent = camera.GetComponent<CameraComponent>();
					cameraComponent.Camera.SetViewportSize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
				}
			}
		}

		switch (m_SceneState)
		{
			case SceneState::Edit:
			{
				m_EditorCamera.OnUpdate(ts);
				m_ActiveScene->OnUpdateEditor(ts, m_EditorCamera);
				break;
			}
			case SceneState::Play:
			{
				m_ActiveScene->OnUpdateRuntime(ts);
				break;
			}
		}
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

				if (ImGui::MenuItem("Save As...", "Control+Shift+S"))
					m_SaveAsScene = true;

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
		m_ContentBrowserPanel.OnImGuiRenderer();

		auto stats = Renderer2D::GetStats();

		ImGui::Begin("Settings");
		static bool lock = false;
		auto renderApi = NAMEOF_ENUM(RendererApi::GetApi());
		ImGui::Text("Render API: %s", renderApi.data());
		ImGui::Checkbox("Lock camera", &lock);
		std::string name = "None";
		if (m_HoveredEntity)
			name = m_HoveredEntity.GetComponent<TagComponent>().Tag;
		ImGui::Text("Hovered Entity: %s", name.c_str());

		ImGui::End(); // Settings

		ImGui::Begin("Stats");
		ImGui::Text("Draw calls: %d", stats.DrawCalls);
		ImGui::Text("Quads: %d", stats.QuadCount);
		ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
		ImGui::Text("Indices: %d", stats.GetTotalIndexCount());

		ImGui::Separator();
		static bool open = false;
		if (ImGui::Button("Show Demo"))
			open = true;

		if (open)
			ImGui::ShowDemoWindow(&open);

		ImGui::End(); // Stats

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("Viewport");

		auto viewportMinRegion = ImGui::GetWindowContentRegionMin();
		auto viewportMaxRegion = ImGui::GetWindowContentRegionMax();
		auto viewportOffset = ImGui::GetWindowPos();
		m_ViewportBounds[0] = { viewportMinRegion.x + viewportOffset.x, viewportMinRegion.y + viewportOffset.y };
		m_ViewportBounds[1] = { viewportMaxRegion.x + viewportOffset.x, viewportMaxRegion.y + viewportOffset.y };

		m_ViewportFocused = ImGui::IsWindowFocused();
		m_ViewportHovered = ImGui::IsWindowHovered();
		Application::Get().GetImGuiLayer()->BlockEvents(!m_ViewportFocused && !m_ViewportHovered);

		ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
		m_ViewportSize = *((glm::vec2*)&viewportPanelSize);

		ImGui::Image((ImTextureID)(intptr_t)m_RenderPassRenderer.Framebuffer->GetColorAttachmentRendererId(), ImVec2{ m_ViewportSize.x, m_ViewportSize.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });

		if(ImGui::BeginDragDropTarget())
		{
			if(const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM"))
			{
				#ifdef PLATFORM_WINDOWS
				const wchar_t* path = (const wchar_t*)payload->Data;
				#else
				const char* path = (const char*)payload->Data;
				#endif
				OpenScene(path);
			}
			ImGui::EndDragDropTarget();
		}

		// Gizmos
		Entity selectedEntity = m_SceneHierarchyPanel.GetSelectedEntity();
		if (selectedEntity && m_GizmoType != -1)
		{
			ImGuizmo::SetOrthographic(false);
			ImGuizmo::SetDrawlist();

			ImGuizmo::SetRect(m_ViewportBounds[0].x, m_ViewportBounds[0].y, m_ViewportBounds[1].x - m_ViewportBounds[0].x, m_ViewportBounds[1].y - m_ViewportBounds[0].y);

			// Camera
			const glm::mat4& cameraProjection = m_EditorCamera.GetProjection();
			glm::mat4 cameraView = m_EditorCamera.GetViewMatrix();

			// Entity transform
			auto& tc = selectedEntity.GetComponent<TransformComponent>();
			glm::mat4 transform = tc.GetTransform();

			// Snapping
			bool snap = Input::IsKeyPressed(KeyCode::LeftControl);
			float snapValue = 0.5f; // Snap to 0.5m for translation/scale
			// Snap to 45 degrees for rotation
			if(m_GizmoType == ImGuizmo::OPERATION::ROTATE)
				snapValue = 45.0f;

			float snapValues[3] = {snapValue, snapValue, snapValue};

			ImGuizmo::Manipulate(glm::value_ptr(cameraView), glm::value_ptr(cameraProjection),
				(ImGuizmo::OPERATION)m_GizmoType, ImGuizmo::LOCAL, glm::value_ptr(transform),
				nullptr, snap ? snapValues : nullptr);

			if (ImGuizmo::IsUsing())
			{
				glm::vec3 translation, rotation, scale;
				Math::DecomposeTransform(transform, translation, rotation, scale);

				auto deltaRotation = rotation - tc.Rotation;
				tc.Translation = translation;
				tc.Rotation += deltaRotation;
				tc.Scale = scale;
			}
		}

		ImGui::End(); // Viewport
		ImGui::PopStyleVar();

		UI_Toolbar();

		ImGui::End(); // Lucky Editor
		ImGui::PopStyleVar(2);

		if (tryLoadLayout < 10)
		{
			Application::Get().GetImGuiLayer()->Load();
			tryLoadLayout++;
		}

		if (m_NewScene)
			NewScene();

		if (m_OpenScene)
			OpenScene();

		if (m_SaveScene)
			SaveScene();

		if (m_SaveAsScene)
			SaveAsScene();

		if (m_LoadLayout)
			LoadLayout();

		if (m_SaveLayout)
			SaveLayout();

		if (m_DuplicateEntity)
			DuplicatEntity();

		m_EditorCamera.Lock(lock);
	}

	void EditorLayer::OnEvent(Event& event)
	{
		if(m_SceneState == SceneState::Play)
			m_ActiveScene->OnEvent(event);
		m_EditorCamera.OnEvent(event);
		EventDispatcher dispatcher(event);
		dispatcher.Dispatch<KeyPressedEvent>(BIND_EVENT_FN(EditorLayer::OnKeyPressed));
		dispatcher.Dispatch<MouseButtonPressedEvent>(BIND_EVENT_FN(EditorLayer::OnMouseButtonPressed));
	}

	void EditorLayer::NewScene()
	{
		m_ActiveScene = InitScene();
		m_ActiveScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
		m_SceneHierarchyPanel.SetContext(m_ActiveScene);

		m_EditorCamera.Reset();
		m_EditorScenePath = std::filesystem::path();
		m_NewScene = false;
	}

	void EditorLayer::OpenScene()
	{
		std::string filePath = Platform::OpenFile("Lucky Scene (*.lucky)\0*.lucky\0All files\0*.*\0\0", STRCAT(ASSETS, "/scenes"));
		if (!filePath.empty())
			OpenScene(filePath);
	}

	void EditorLayer::OpenScene(const std::filesystem::path& filePath)
	{
		m_GizmoType = -1;
		if (m_SceneState == SceneState::Play)
			OnSceneStop();

		if (!filePath.empty())
		{
			if (filePath != "##Cancel")
			{
				if (filePath.extension().string() != ".lucky")
				{
					LK_WARN("Could not load {0} - not a scene file", filePath.filename().string());
					return;
				}

				auto scene = InitScene();
				SceneSerializer serializer(scene);
				if(serializer.Deserialize(filePath.string()))
				{
					m_EditorScene = scene;
					m_EditorScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
					m_SceneHierarchyPanel.SetContext(m_EditorScene);

					m_ActiveScene = m_EditorScene;
					m_EditorScenePath = filePath;

					auto cameraEntity = m_ActiveScene->GetPrimaryCamera();
					if (cameraEntity)
						cameraEntity.AddComponent<NativeScriptComponent>().Bind<CameraScript>();
				}
			}
			m_OpenScene = false;
		}
	}

	void EditorLayer::SaveScene()
	{
		if (!m_EditorScenePath.empty())
			SerializeScene(m_ActiveScene, m_EditorScenePath);
		else
			SaveAsScene();
		m_SaveScene = false;
	}

	void EditorLayer::SaveAsScene()
	{
		std::string filePath = Platform::SaveFile("Lucky Scene (*.lucky)\0*.lucky;\0\0", STRCAT(ASSETS, "/scenes"));
		if (!filePath.empty())
		{
			if (filePath != "##Cancel")
			{
				SerializeScene(m_ActiveScene, filePath);
				m_EditorScenePath = filePath;
			}
			m_SaveAsScene = false;
		}
	}

	void EditorLayer::SerializeScene(Ref<Scene> scene, const std::filesystem::path& filePath)
	{
		SceneSerializer serializer(scene);
		serializer.Serialize(filePath.string());
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

	void EditorLayer::DuplicatEntity()
	{
		if (m_SceneState == SceneState::Edit)
		{
			auto selectedEntity = m_SceneHierarchyPanel.GetSelectedEntity();
			if (selectedEntity)
				m_EditorScene->DuplicateEntity(selectedEntity);
		}
		m_DuplicateEntity = false;
	}

	bool EditorLayer::OnKeyPressed(KeyPressedEvent& e)
	{
		if (m_SceneState == SceneState::Play)
			return false;

		if (e.GetRepeatCount() > 0)
			return false;

		bool alt = Input::IsKeyPressed(KeyCode::LeftAlt) || Input::IsKeyPressed(KeyCode::RightAlt);
		bool control = Input::IsKeyPressed(KeyCode::LeftControl) || Input::IsKeyPressed(KeyCode::RightControl);
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
			{
				if(control)
					m_SaveAsScene = true;
				else
					m_SaveScene = true;
			}
			if (alt)
				m_SaveLayout = true;
			break;
		}

		case KeyCode::C:
		{
			if (shift)
				m_DuplicateEntity = true;
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

		case KeyCode::D1:
		{
			m_GizmoType = -1;
			break;
		}

		case KeyCode::D2:
		{
			m_GizmoType = ImGuizmo::OPERATION::TRANSLATE;
			break;
		}

		case KeyCode::D3:
		{
			m_GizmoType = ImGuizmo::OPERATION::ROTATE;
			break;
		}

		case KeyCode::D4:
		{
			m_GizmoType = ImGuizmo::OPERATION::SCALE;
			break;
		}

		default:
			break;
		}

		return false;
	}

	bool EditorLayer::OnMouseButtonPressed(MouseButtonEvent& e)
	{
		if(e.GetButton() == MouseButton::ButtonLeft)
		{
			if (CanPick())
				m_SceneHierarchyPanel.SetSelectedEntity(m_HoveredEntity);
		}
		return false;
	}

	Ref<Scene> EditorLayer::InitScene()
	{
		auto& shaderLibrary = ShaderLibrary::GetInstance();

		Ref<Scene> scene = CreateRef<Scene>();

		FramebufferSpecification fbSpec;
		if(RendererApi::GetApi() == RendererApi::Api::OpenGL)
		{
			fbSpec.AttachmentSpecs = { FramebufferTextureFormat::RGBA8, FramebufferTextureFormat::RED_INTEGER,  FramebufferTextureFormat::Depth };
			fbSpec.Width = (uint32_t)m_ViewportSize.x;
			fbSpec.Height = (uint32_t)m_ViewportSize.y;
			m_RenderPassRenderer.Name = "Renderer";
			m_RenderPassRenderer.Framebuffer = Framebuffer::Create(fbSpec);
			m_RenderPassRenderer.Shader = shaderLibrary.Get("2DTexture");
			m_RenderPassRenderer.BeforeRenderCallback = [](const auto& pass)
				{
					if (pass.Name == "Renderer")
					{
						RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
						RenderCommand::Clear();
						pass.Framebuffer->ClearAttachment(1, -1);
					}
				};
			m_RenderPassRenderer.AfterRenderCallback = [this](const auto& pass)
				{
					if (pass.Name == "Renderer")
					{
						auto [mx, my] = ImGui::GetMousePos();
						mx -= m_ViewportBounds[0].x;
						my -= m_ViewportBounds[0].y;
						glm::vec2 viewportSize = m_ViewportBounds[1] - m_ViewportBounds[0];

						int mouseX = (int)mx;
						int mouseY = (int)(viewportSize.y - my);

						if (mouseX >= 0 && mouseY >= 0 && mouseX < viewportSize.x && mouseY < viewportSize.y)
						{
							auto entityId = pass.Framebuffer->ReadPixel(1, mouseX, mouseY);
							m_HoveredEntity = entityId == -1 ? Entity() : Entity((entt::entity)entityId, m_ActiveScene.get());
						}
					}
				};

			scene->AddPass(m_RenderPassRenderer);
		}
		else
		{
			fbSpec.AttachmentSpecs = { FramebufferTextureFormat::RGBA8, FramebufferTextureFormat::Depth };
			fbSpec.Width = (uint32_t)m_ViewportSize.x;
			fbSpec.Height = (uint32_t)m_ViewportSize.y;
			m_RenderPassRenderer.Name = "Renderer";
			m_RenderPassRenderer.Framebuffer = Framebuffer::Create(fbSpec);
			m_RenderPassRenderer.Shader = shaderLibrary.Get("2DTexture");
			m_RenderPassRenderer.BeforeRenderCallback = [](const auto& pass)
				{
					if (pass.Name == "Renderer")
					{
						RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
						RenderCommand::Clear();
					}
				};
			m_RenderPassRenderer.CameraUniformBuffer = UniformBuffer::Create("Camera", m_RenderPassRenderer.Shader.get(), 0);

			FramebufferSpecification fbSpec2;
			fbSpec2.AttachmentSpecs = { FramebufferTextureFormat::RED_INTEGER, FramebufferTextureFormat::Depth };
			fbSpec2.Width = (uint32_t)m_ViewportSize.x;
			fbSpec2.Height = (uint32_t)m_ViewportSize.y;
			m_RenderPassMousePicking.Name = "MousePicking";
			m_RenderPassMousePicking.Framebuffer = Framebuffer::Create(fbSpec2);
			m_RenderPassMousePicking.Shader = shaderLibrary.Get("MousePicking");
			m_RenderPassMousePicking.BeforeRenderCallback = [](const auto& pass)
				{
					if (pass.Name == "MousePicking")
					{
						pass.Framebuffer->ClearAttachment(0, -1);
						RenderCommand::ClearDepth();
					}
				};
			m_RenderPassMousePicking.AfterRenderCallback = [this](const auto& pass)
				{
					if (pass.Name == "MousePicking")
					{
						auto [mx, my] = ImGui::GetMousePos();
						mx -= m_ViewportBounds[0].x;
						my -= m_ViewportBounds[0].y;
						glm::vec2 viewportSize = m_ViewportBounds[1] - m_ViewportBounds[0];

						int mouseX = (int)mx;
						int mouseY = (int)(viewportSize.y - my);

						if (mouseX >= 0 && mouseY >= 0 && mouseX < viewportSize.x && mouseY < viewportSize.y)
						{
							auto entityId = pass.Framebuffer->ReadPixel(0, mouseX, mouseY);
							m_HoveredEntity = entityId == -1 ? Entity() : Entity((entt::entity)entityId, m_ActiveScene.get());
						}
					}
				};
			m_RenderPassMousePicking.CameraUniformBuffer = UniformBuffer::Create("Camera", m_RenderPassRenderer.Shader.get(), 0);

			scene->AddPass(m_RenderPassMousePicking);
			scene->AddPass(m_RenderPassRenderer);
		}

		return scene;
	}

	void EditorLayer::UI_Toolbar()
	{
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 2));
		ImGui::PushStyleVar(ImGuiStyleVar_ItemInnerSpacing, ImVec2(0, 0));
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
		auto& colors = ImGui::GetStyle().Colors;
		const auto& buttonHovered = colors[ImGuiCol_ButtonHovered];
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(buttonHovered.x, buttonHovered.y, buttonHovered.z, 0.5f));
		const auto& buttonActive = colors[ImGuiCol_ButtonActive];
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(buttonActive.x, buttonActive.y, buttonActive.z, 0.5f));

		ImGui::Begin("##toolbar", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);

		float size = ImGui::GetWindowHeight() - 4.0f;
		Ref<Texture2D> icon = m_SceneState == SceneState::Edit ? m_IconPlay : m_IconStop;
		ImGui::SetCursorPosX((ImGui::GetWindowContentRegionMax().x * 0.5f) - (size * 0.5f));
		if (ImGui::ImageButton((ImTextureID)(intptr_t)icon->GetRendererId(), ImVec2(size, size), ImVec2(0, 0), ImVec2(1, 1), 0))
		{
			if (m_SceneState == SceneState::Edit)
				OnScenePlay();
			else if (m_SceneState == SceneState::Play)
				OnSceneStop();
		}
		ImGui::PopStyleVar(2);
		ImGui::PopStyleColor(3);
		ImGui::End();
	}

	void EditorLayer::OnScenePlay()
	{
		m_ActiveScene = Scene::Copy(m_EditorScene);
		m_ActiveScene->OnRuntimeStart();
		m_SceneHierarchyPanel.SetContext(m_ActiveScene);
		m_GizmoType = -1;
		m_SceneState = SceneState::Play;
	}

	void EditorLayer::OnSceneStop()
	{
		m_ActiveScene->OnRuntimeStop();
		m_ActiveScene = m_EditorScene;
		m_SceneHierarchyPanel.SetContext(m_ActiveScene);
		m_SceneState = SceneState::Edit;
	}
}
