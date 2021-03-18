#include "sandpch.h"
#include "PhysicsWorld.h"

namespace Sand
{

	PhysicsWorld::PhysicsWorld(const glm::vec2& gravity)
	{
	}

	void PhysicsWorld::Create(const glm::vec2& gravity)
	{
		m_World = new b2World({ gravity.x, gravity.y });
	}

	void PhysicsWorld::Step()
	{
		SAND_PROFILE_FUNCTION();

		constexpr int velocityIterations = 8, positionIterations = 3;

		m_World->Step(m_FixedTimestep, velocityIterations, positionIterations);
	}

	void PhysicsWorld::Destroy()
	{
		delete m_World;
	}

}