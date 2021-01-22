#pragma once

#include "box2d/box2d.h"

namespace Sand
{

	class PhysicsWorld
	{
	public:
		PhysicsWorld() = delete;

		static void Step();
		static b2Body* CreateBody(const b2BodyDef* fixtureDef);
		static void DestroyBody(b2Body* body);
	};

}