#pragma once

#include <entt.hpp>
#include "Scene.h"

namespace Lucky
{
	class Entity
	{
	public:
		Entity() = default;
		Entity(const Entity& other) = default;
		Entity(entt::entity handle, Scene* scene);

		template<typename T, typename... Args>
		T& AddComponent(Args&&... args)
		{
			LK_CORE_ASSERT(!HasComponent<T>(), "Entity already has component");
			return m_Scene->m_Registry.emplace<T>(m_Handle, std::forward<Args>(args)...);
		}

		template<typename... Args>
		decltype(auto) GetComponent() const
		{
			return m_Scene->m_Registry.get<Args...>(m_Handle);
		}

		template<typename T>
		void RemoveComponent() const
		{
			LK_CORE_ASSERT(HasComponent<T>(), "Entity doen't have component");
			m_Scene->m_Registry.remove<T>(m_Handle);
		}

		template<typename T>
		bool HasComponent() const
		{
			return m_Scene->m_Registry.try_get<T>(m_Handle) != nullptr;
		}

		operator bool() const { return m_Handle != entt::null; }

	private:
		entt::entity m_Handle = entt::null;
		Scene* m_Scene = nullptr;
	};
}
