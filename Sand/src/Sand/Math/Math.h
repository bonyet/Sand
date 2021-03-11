#pragma once

#include <glm/glm.hpp>

namespace Sand::Math
{

	bool DecomposeTransform(const glm::mat4& transform, glm::vec3& outTranslation, glm::vec3& outScale, glm::vec3& outRotation);
	bool DecomposeTransform(const glm::mat4& transform, glm::vec2& outTranslation, glm::vec2& outScale, float& outRotation);
	
	float Clamp(float value, float min, float max);

}