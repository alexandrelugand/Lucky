#pragma once

#include "Entity.h"

namespace Lucky
{
	class ScriptableEntity
	{
	public:
		ScriptableEntity() = default;

		template<typename... Args>
		decltype(auto) GetComponent() const
		{
			return m_Entity.GetComponent<Args...>();
		}

	private:
		Entity m_Entity;
		friend class Scene;
	};
}
