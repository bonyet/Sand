#pragma once

#include <glm/glm.hpp>

namespace Sand
{

	struct Particle
	{
		glm::vec2 Position{ 0.0f, 0.0f }, Velocity{ 0.0f, 0.0f };
		glm::vec2 ScaleBegin{ 1.0f, 1.0f }, ScaleEnd{ 0.0f, 0.0f };
		float Rotation = 0.0f;
		bool RotateClockwise = true;

		float Lifetime = 1.0f, LifeRemaining = 0.0f;
		bool Alive = false;
	};

}