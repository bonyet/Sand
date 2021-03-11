#pragma once

#include "Sand/Core/Time.h"
#include <glm/glm.hpp>

namespace Sand
{

	class Scene;

	class PhysicsWorld
	{
	public:
		PhysicsWorld() = default;

		void SetGravity(const glm::vec2& gravity) { mGravity = gravity; }
		glm::vec2& GetGravity() { return mGravity; }

		void ShowDebugWindow(Scene* const scene);
	private:
		void InitializeBodies(Scene* const scene);

		void Step(Scene* const scene, Timestep timestep);
	private:
		glm::vec2 mGravity = { 0.0f, 0.0f };
		friend class Scene;
	};

}