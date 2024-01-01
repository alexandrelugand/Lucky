#pragma once

#include "Entity.h"

namespace Lucky
{
	class ScriptableEntity
	{
	public:
		ScriptableEntity(const Entity& entity)
			: m_Entity(entity) {}
		virtual ~ScriptableEntity() = default;

		template<typename... Args>
		decltype(auto) GetComponent() const
		{
			return m_Entity.GetComponent<Args...>();
		}

		virtual void OnCreate() {}
		virtual void OnDestroy() {}
		virtual void OnUpdate(Timestep ts) {}
		virtual void OnEvent(Event& e) {}

	private:
		Entity m_Entity;
	};
}
