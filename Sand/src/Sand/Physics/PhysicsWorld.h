#pragma once

#include <glm/glm.hpp>
#include <box2d/box2d.h>

namespace Sand
{

	class Scene;

	class PhysicsWorld
	{
	public:
		PhysicsWorld(const glm::vec2& gravity = { 0, 0 });

		b2World* GetWorld() { return m_World; }
	private:
		void Create(const glm::vec2& gravity = { 0.0f, -10.0f });
		void Step();
		void Destroy();
	private:
		b2World* m_World = nullptr;
		float m_FixedTimestep = 1.0f / 60.0f;

		friend class Scene;
	};

}