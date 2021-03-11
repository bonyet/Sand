#include "sandpch.h"
#include "Actor.h"

namespace Sand
{

	Actor::Actor(entt::entity handle, Scene* scene)
		: mEntityHandle(handle), mScene(scene)
	{
	}

}