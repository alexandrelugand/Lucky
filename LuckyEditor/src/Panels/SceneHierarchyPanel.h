#pragma once

#include <Lucky.h>

namespace Lucky
{
	class SceneHierarchyPanel
	{
	public:
		SceneHierarchyPanel() = default;
		SceneHierarchyPanel(const Ref<Scene> context);

		void SetContext(const Ref<Scene>& context);
		void OnImGuiRenderer();

		Entity GetSelectedEntity() const { return m_SelectedEntity; }
		void SetSelectedEntity(Entity entity) { m_SelectedEntity = entity; }

	private:
		void DrawEntityNode(const Entity& entity);
		void DrawComponents(const Entity& entity);

		Ref<Scene> m_Context;
		Entity m_SelectedEntity;

		friend class Scene;
	};
}
