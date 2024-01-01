#include "LuckyEditorPch.h"
#include "EditorLayer.h"

#include <glm/gtx/matrix_decompose.hpp>

namespace Lucky
{
	EditorLayer::EditorLayer()
		: Layer("EditorLayer")
	{
		m_ActiveScene = CreateScope<Scene>();
	}

	void EditorLayer::OnAttach()
	{
		LK_PROFILE_FUNCTION();

		auto& window = Application::Get().GetWindow();

		CameraSettings settings;
		settings.AspectRatio = (float)window.GetWidth() / (float)window.GetHeight();
		settings.EnableRotation = true;
		settings.TranslationSpeed = 10.0f;
		settings.RotationSpeed = 90.0f;
		settings.ZoomSpeed = 0.5f;
		settings.ZoomLevel = 1.0f;
		settings.Size = 10.0f;
		settings.NearClip = -1.0f;
		settings.FarClip = 1.0f;

		FramebufferSpecification fbSpec;
		fbSpec.Width = window.GetWidth();
		fbSpec.Height = window.GetHeight();
		m_Framebuffer = Framebuffer::Create(fbSpec);

		class CameraController : public ScriptableEntity
		{
			const float c_TranslationSpeed = 5.0f;
			const float c_RotationSpeed = 90.0f;

		public:
			void OnCreate()
			{				
			}

			void OnDestroy()
			{
			}

			void OnUpdate(Timestep ts)
			{
				auto& transform = GetComponent<TransformComponent>().Transform;

				glm::vec3 scale;
				glm::quat rotation;
				glm::vec3 translation;
				glm::vec3 skew;
				glm::vec4 perspective;
				glm::decompose(transform, scale, rotation, translation, skew, perspective);
				rotation = glm::conjugate(rotation);
				glm::vec3 angles = glm::eulerAngles(rotation);
				float angle = -glm::degrees(angles.z);

#ifndef __EMSCRIPTEN__
				if (Input::IsKeyPressed(KeyCode::A))
#else
				if (Input::IsKeyPressed(KeyCode::Q))
#endif
				{
					translation.x -= cos(glm::radians(angle)) * c_TranslationSpeed * ts;
					translation.y -= sin(glm::radians(angle)) * c_TranslationSpeed * ts;
				}
				else
					if (Input::IsKeyPressed(KeyCode::D))
					{
						translation.x += cos(glm::radians(angle)) * c_TranslationSpeed * ts;
						translation.y += sin(glm::radians(angle)) * c_TranslationSpeed * ts;
					}

				if (Input::IsKeyPressed(KeyCode::S))
				{
					translation.x -= -sin(glm::radians(angle)) * c_TranslationSpeed * ts;
					translation.y -= cos(glm::radians(angle)) * c_TranslationSpeed * ts;
				}
				else
#ifndef __EMSCRIPTEN__
					if (Input::IsKeyPressed(KeyCode::W))
#else
					if (Input::IsKeyPressed(KeyCode::Z))
#endif
					{
						translation.x += -sin(glm::radians(angle)) * c_TranslationSpeed * ts;
						translation.y += cos(glm::radians(angle)) * c_TranslationSpeed * ts;
					}


					if (Input::IsKeyPressed(KeyCode::E))
						angle -= c_RotationSpeed * ts;
					else
#ifndef __EMSCRIPTEN__
						if (Input::IsKeyPressed(KeyCode::Q))
#else
						if (Input::IsKeyPressed(KeyCode::A))
#endif
							angle += c_RotationSpeed * ts;

					if (angle > 180.0f)
						angle -= 360.0f;
					else if (angle <= -180.0f)
						angle += 360.0f;

				if (Input::IsKeyPressed(KeyCode::Space))
				{
					translation = glm::vec3(0.0f);
					angle = 0.0f;
				}

				transform = glm::translate(glm::mat4(1.0f), translation) *
					glm::rotate(glm::mat4(1.0f), glm::radians(angle), glm::vec3{ 0.0f, 0.0f, 1.0f }) *
					glm::scale(glm::mat4(1.0f), glm::vec3{ scale.x, scale.y, 1.0f });
			}

			void OnEvent(Event& e)
			{
			}
		};

		m_CameraAEntity = m_ActiveScene->CreateEntity("Camera A");
		m_CameraAEntity.AddComponent<CameraComponent>(settings).Primary = true;
		m_CameraAEntity.AddComponent<NativeScriptComponent>().Bind<CameraController>();

		m_CameraBEntity = m_ActiveScene->CreateEntity("Camera B");
		m_CameraBEntity.AddComponent<CameraComponent>(settings);

		m_SquareEntity = m_ActiveScene->CreateEntity("Square");
		m_SquareEntity.AddComponent<SpriteRendererComponent>(glm::vec4{ 0.0f, 1.0f, 0.0f, 1.0f });

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

		ImGui::Begin("Settings");
		if(m_SquareEntity)
		{			
			ImGui::Text("%s", m_SquareEntity.GetComponent<TagComponent>().Tag.c_str());
			const auto& [transform, sprite] = m_SquareEntity.GetComponent<TransformComponent, SpriteRendererComponent>();
			ImGui::ColorEdit4("Color", glm::value_ptr(sprite.Color));

			glm::vec3 scale;
			glm::quat rotation;
			glm::vec3 translation;
			glm::vec3 skew;
			glm::vec4 perspective;
			glm::decompose(transform.Transform, scale, rotation, translation, skew, perspective);
			rotation = glm::conjugate(rotation);
			glm::vec3 angles = glm::eulerAngles(rotation);
			float angle = -glm::degrees(angles.z);

			ImGui::DragFloat2("Position", glm::value_ptr(translation), 0.1f, -100.0f, 100.0f, "%.1f", ImGuiSliderFlags_AlwaysClamp);
			ImGui::DragFloat("Rotation", &angle, 1.0f, -360.0f, 360.0f, "%.1f", ImGuiSliderFlags_AlwaysClamp);
			ImGui::DragFloat2("Size", glm::value_ptr(scale), 0.1f, 0.1f, 100.0f, "%.1f", ImGuiSliderFlags_AlwaysClamp);

			transform = glm::translate(glm::mat4(1.0f), translation) *
				glm::rotate(glm::mat4(1.0f), glm::radians(angle), glm::vec3{ 0.0f, 0.0f, 1.0f }) *
				glm::scale(glm::mat4(1.0f), glm::vec3{ scale.x, scale.y, 1.0f });
		}

		ImGui::DragFloat3("Camera transform", glm::value_ptr(m_CameraAEntity.GetComponent<TransformComponent>().Transform[3]));

		if(ImGui::Checkbox("Camera A", &m_CameraA))
		{
			m_CameraAEntity.GetComponent<CameraComponent>().Primary = m_CameraA;
			m_CameraBEntity.GetComponent<CameraComponent>().Primary = !m_CameraA;
		}

		auto& camera = m_CameraBEntity.GetComponent<CameraComponent>().Camera;
		float orthoSize = camera.GetOrthographicSize();
		if (ImGui::DragFloat("CamB ortho size", &orthoSize))
			camera.SetOrthographicSize(orthoSize);

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
