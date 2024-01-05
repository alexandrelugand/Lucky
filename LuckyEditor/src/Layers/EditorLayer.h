#pragma once
#include <Lucky.h>

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
		void SaveScene();
		void LoadLayout();
		void SaveLayout();
		bool OnKeyPressed(KeyPressedEvent& e);

		Ref<Scene> m_ActiveScene;
		Ref<Framebuffer> m_Framebuffer;
		glm::vec2 m_ViewportSize = { 1.0f, 1.0f };
		bool m_ViewportFocused = false, m_ViewportHovered = false;
		bool m_NewScene = false, m_OpenScene = false, m_SaveScene = false, m_SaveLayout = false, m_LoadLayout = false;
		SceneHierarchyPanel m_SceneHierarchyPanel;
		int m_GizmoType = -1;
	};
} // namespace Lucky
