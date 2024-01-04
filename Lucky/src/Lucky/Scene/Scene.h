#pragma once

#include <entt.hpp>

#include "Lucky/Core/Timestep.h"
#include "Lucky/Core/Events/Event.h"

namespace Lucky
{
	class Entity;

	class Scene
	{
	public:
		Scene();
		~Scene();

		Entity CreateEntity(const std::string& name = std::string());
		void DestroyEntity(Entity entity);
		void Clean();

		void OnUpdate(Timestep ts);
		void OnEvent(Event& e);

		void OnViewportResize(uint32_t width, uint32_t height);

	private:
		template<typename T>
		void OnComponentAdded(Entity entity, T& component);

		entt::registry m_Registry;
		uint32_t m_ViewportWidth = 1, m_ViewportHeight = 1;

		friend class Entity;
		friend class SceneHierarchyPanel;
		friend class SceneSerializer;
	};	
}
