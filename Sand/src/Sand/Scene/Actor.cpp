#include "sandpch.h"
#include "Actor.h"

namespace Sand
{

	Actor::Actor(entt::entity handle, Scene* scene)
		: m_EntityHandle(handle), m_Scene(scene)
	{
	}

}