#include "sandpch.h"
#include "PhysicsWorld.h"

namespace Sand
{
	static b2World s_World = b2World({ 0.0f, -10.0f });
	static const float s_FixedTimestep = 1.0f / 40.0f;
	static const uint32_t s_VelocityIterations = 6;
	static const uint32_t s_PositionIterations = 2;

	void PhysicsWorld::Step()
	{
		s_World.Step(s_FixedTimestep, s_VelocityIterations, s_PositionIterations);
	}

	b2Body* PhysicsWorld::CreateBody(const b2BodyDef* bodyDef)
	{
		return s_World.CreateBody(bodyDef);
	}

	void PhysicsWorld::DestroyBody(b2Body* body)
	{
		s_World.DestroyBody(body);
	}

}