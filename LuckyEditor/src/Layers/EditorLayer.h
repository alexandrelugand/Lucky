#pragma once
#include <Lucky.h>

#include <ImGuizmo.h>

#include "Panels/ContentBrowserPanel.h"
#include "Panels/SceneHierarchyPanel.h"

namespace Lucky
{
	class EditorLayer : public Layer
	{
	public:
		EditorLayer();

		void OnAttach() override;
		void OnDetach() override;
		void OnUpdate(Timestep ts) override;
		void OnImGuiRender() override;
		void OnEvent(Event &event) override;

	private:
		void NewScene();
		void OpenScene();
		void OpenScene(const std::filesystem::path& filePath);
		void SaveScene();
		void SaveAsScene();
		void SerializeScene(Ref<Scene> scene, const std::filesystem::path& filePath);
		void LoadLayout();
		void SaveLayout();
		void DuplicatEntity();
		bool OnKeyPressed(KeyPressedEvent& e);
		bool OnMouseButtonPressed(MouseButtonEvent& e);
		Ref<Scene> InitScene();
		bool CanPick() const { return m_ViewportHovered && !ImGuizmo::IsOver() && !Input::IsKeyPressed(KeyCode::LeftAlt); }
		void OnOverlayRender();

		// UI Toolbar
		void UI_Toolbar();
		void OnScenePlay();
		void OnSceneStop();

		Ref<Scene> m_ActiveScene;
		Ref<Scene> m_EditorScene;
		std::filesystem::path m_EditorScenePath;
		RenderPass m_RenderPassRenderer;
		RenderPass m_RenderPassMousePicking;

		glm::vec2 m_ViewportSize = { 1.0f, 1.0f };
		glm::vec2 m_ViewportBounds[2];

		bool m_ViewportFocused = false, m_ViewportHovered = false;
		bool m_NewScene = false, m_OpenScene = false, m_SaveScene = false, m_SaveAsScene = false, m_DuplicateEntity = false, m_SaveLayout = false, m_LoadLayout = false;
		SceneHierarchyPanel m_SceneHierarchyPanel;
		ContentBrowserPanel m_ContentBrowserPanel;
		int m_GizmoType = -1;
		bool m_ShowPhysicsColliders = false;

		EditorCamera m_EditorCamera;
		Entity m_HoveredEntity{};

		enum class SceneState
		{
			Edit = 0,
			Play
		};

		SceneState m_SceneState = SceneState::Edit;
		Ref<Texture2D> m_IconPlay, m_IconStop;
	};
}
