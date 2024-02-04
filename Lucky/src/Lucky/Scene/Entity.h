#pragma once

#include "Components.h"
#include "Scene.h"

#include <entt.hpp>

namespace Lucky
{
	class Entity
	{
	public:
		Entity() = default;
		Entity(entt::entity handle, Scene* scene);
		Entity(const Entity& other);

		template<typename T, typename... Args>
		T& AddComponent(Args&&... args)
		{
			LK_CORE_ASSERT(!HasComponent<T>(), "Entity already has component");
			T& component = m_Scene->m_Registry.emplace<T>(m_Handle, std::forward<Args>(args)...);
			m_Scene->OnComponentAdded(*this, component);
			return component;
		}


		template<typename T, typename... Args>
		T& AddOrReplaceComponent(Args&&... args)
		{
			T& component = m_Scene->m_Registry.emplace_or_replace<T>(m_Handle, std::forward<Args>(args)...);
			m_Scene->OnComponentAdded(*this, component);
			return component;
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

		Uuid GetUuid() const { return GetComponent<IdComponent>().Id; }
		std::string GetName() const { return GetComponent<TagComponent>().Tag; }
		glm::mat4 GetTransform() const { return GetComponent<TransformComponent>().GetTransform(); }

		operator bool() const { return m_Handle != entt::null; }
		operator entt::entity() const { return m_Handle; }
		operator uint32_t() const { return (uint32_t)m_Handle; }

		bool operator ==(const Entity& other) const { return m_Handle == other.m_Handle && m_Scene == other.m_Scene; }
		bool operator !=(const Entity& other) const { return !(*this == other); }

	private:
		entt::entity m_Handle = entt::null;
		Scene* m_Scene = nullptr;
	};
}
