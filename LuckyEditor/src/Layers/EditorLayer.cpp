#include "LuckyEditorPch.h"
#include "EditorLayer.h"

#include "Lucky/Platforms/Platform.h"
#include "ImGuizmo.h"
#include "Lucky/Math/Math.h"

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

		InitScene();

#if 0
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

		for (auto& pass : m_ActiveScene->GetPasses())
		{
			auto& fbSpec = pass.Framebuffer->GetSpecification();
			if (m_ViewportSize.x > 0 && m_ViewportSize.y > 0 &&
				(fbSpec.Width != m_ViewportSize.x || fbSpec.Height != m_ViewportSize.y))
			{
				pass.Framebuffer->Resize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
				m_EditorCamera.SetViewportSize(m_ViewportSize.x, m_ViewportSize.y);
				m_ActiveScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
			}
		}

		m_EditorCamera.OnUpdate(ts);

		//m_Framebuffer->Bind();

		// Prepare scene
		RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
		RenderCommand::Clear();

#ifndef __EMSCRIPTEN__
		// Clear our entity Id attachment to -1
		//m_Framebuffer->ClearAttachment(1, -1);
#endif

		m_ActiveScene->OnUpdateEditor(ts, m_EditorCamera);

#ifndef __EMSCRIPTEN__
		/*auto [mx, my] = ImGui::GetMousePos();
		mx -= m_ViewportBounds[0].x;
		my -= m_ViewportBounds[0].y;
		glm::vec2 viewportSize = m_ViewportBounds[1] - m_ViewportBounds[0];

		int mouseX = (int)mx;
		int mouseY = (int)(viewportSize.y - my);

		if (mouseX >= 0 && mouseY >= 0 && mouseX < viewportSize.x && mouseY < viewportSize.y)
		{
			auto entityId = m_Framebuffer->ReadPixel(1, mouseX, mouseY);
			m_HoveredEntity = entityId == -1 ? Entity() : Entity((entt::entity)entityId, m_ActiveScene.get());
		}*/
#endif

		//m_Framebuffer->Unbind();
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

		ImGui::Begin("Settings");
		static bool lock = false;;
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
		auto viewportOffset = ImGui::GetCursorPos(); // Includes tab bar

		m_ViewportFocused = ImGui::IsWindowFocused();
		m_ViewportHovered = ImGui::IsWindowHovered();
		Application::Get().GetImGuiLayer()->BlockEvents(!m_ViewportFocused && !m_ViewportHovered);

		ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
		m_ViewportSize = *((glm::vec2*)&viewportPanelSize);

		ImGui::Image((ImTextureID)(intptr_t)m_RenderPassRenderer.Framebuffer->GetColorAttachmentRendererId(), ImVec2{ m_ViewportSize.x, m_ViewportSize.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });

		auto windowSize = ImGui::GetWindowSize();
		ImVec2 minBound = ImGui::GetWindowPos();
		minBound.x += viewportOffset.x;
		minBound.y += viewportOffset.y;

		ImVec2 maxBound = { minBound.x + windowSize.x, minBound.y + windowSize.y };
		m_ViewportBounds[0] = { minBound.x, minBound.y };
		m_ViewportBounds[1] = { maxBound.x, maxBound.y };

		// Gizmos
		Entity selectedEntity = m_SceneHierarchyPanel.GetSelectedEntity();
		if (selectedEntity && m_GizmoType != -1)
		{
			ImGuizmo::SetOrthographic(false);
			ImGuizmo::SetDrawlist();

			float windowWidth = ImGui::GetWindowWidth();
			float windowHeight = ImGui::GetWindowHeight();
			auto windowPos = ImGui::GetWindowPos();
			ImGuizmo::SetRect(windowPos.x, windowPos.y, windowWidth, windowHeight);

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

		if (m_LoadLayout)
			LoadLayout();

		if (m_SaveLayout)
			SaveLayout();

		m_EditorCamera.Lock(lock);
	}

	void EditorLayer::OnEvent(Event& event)
	{
		m_ActiveScene->OnEvent(event);
		m_EditorCamera.OnEvent(event);
		EventDispatcher dispatcher(event);
		dispatcher.Dispatch<KeyPressedEvent>(BIND_EVENT_FN(EditorLayer::OnKeyPressed));
	}

	void EditorLayer::NewScene()
	{
		InitScene();
		/*m_ActiveScene = CreateRef<Scene>();
		m_ActiveScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
		m_SceneHierarchyPanel.SetContext(m_ActiveScene);*/
		m_EditorCamera.Reset();
		m_NewScene = false;
	}

	void EditorLayer::OpenScene()
	{
		std::string filePath = Platform::OpenFile("Lucky Scene (*.lucky)\0*.lucky\0All files\0*.*\0\0", STRCAT(ASSETS, "/scenes"));
		if (!filePath.empty())
		{
			if (filePath != "##Cancel")
			{
				InitScene();

				/*m_ActiveScene = CreateRef<Scene>();
				m_ActiveScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
				m_SceneHierarchyPanel.SetContext(m_ActiveScene);*/

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

	void EditorLayer::InitScene()
	{
		auto& window = Application::Get().GetWindow();
		auto& shaderLibrary = ShaderLibrary::GetInstance();

		m_ActiveScene = CreateRef<Scene>();

		FramebufferSpecification fbSpec;
		fbSpec.AttachmentSpecs = { FramebufferTextureFormat::RGBA8, FramebufferTextureFormat::Depth };
		fbSpec.Width = window.GetWidth();
		fbSpec.Height = window.GetHeight();
		m_RenderPassRenderer.Name = "Renderer";
		m_RenderPassRenderer.Framebuffer = Framebuffer::Create(fbSpec);
		m_RenderPassRenderer.Shader = shaderLibrary.Get("2DTexture");
		m_RenderPassRenderer.BeforeRenderCallback = [](const auto& pass)
		{
			if(pass.Name == "Renderer")
			{
				RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
				RenderCommand::Clear();
			}
		};

		FramebufferSpecification fbSpec2;
		fbSpec2.AttachmentSpecs = { FramebufferTextureFormat::RED_INTEGER, FramebufferTextureFormat::Depth };
		fbSpec2.Width = window.GetWidth();
		fbSpec2.Height = window.GetHeight();
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

		m_ActiveScene->AddPass(m_RenderPassMousePicking);
		m_ActiveScene->AddPass(m_RenderPassRenderer);

		m_ActiveScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
		m_SceneHierarchyPanel.SetContext(m_ActiveScene);
	}
} // namespace Lucky
