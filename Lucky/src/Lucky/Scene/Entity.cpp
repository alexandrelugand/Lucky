#include "LuckyPch.h"
#include "Entity.h"

namespace Lucky
{
	Entity::Entity(entt::entity handle, Scene* scene)
		: m_Handle(handle), m_Scene(scene)
	{
	}
}
