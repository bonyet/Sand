#pragma once

#include <glm/glm.hpp>

namespace Sand
{ namespace Math {

	bool DecomposeTransform(const glm::mat4& transform, glm::vec3& outTranslation, glm::vec3& outScale, glm::vec3& outRotation);

} }