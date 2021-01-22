#pragma once

#include <glm/glm.hpp>

namespace Sand
{ namespace Math {

	bool DecomposeTransform(const glm::mat4& transform, glm::vec3& outTranslation, glm::vec3& outScale, glm::vec3& outRotation);
	
	float Clamp(float value, float min, float max);

	class FVector
	{
	public:
		FVector(float x, float y, float z)
			: x(x), y(y), z(z) { }
		FVector(float scalar)
			: x(scalar), y(scalar), z(scalar) { }
		FVector(const glm::vec3& glmVector)
			: x(glmVector.x), y(glmVector.y), z(glmVector.z) { }
		
		float x, y, z;
	public:
		float GetLength() const
		{
			return glm::length(static_cast<glm::vec3>(*this));
		}
		FVector Dot(const FVector& other) const
		{
			return glm::dot(static_cast<glm::vec3>(*this), static_cast<glm::vec3>(other));
		}
		float Distance(const FVector& other) const
		{
			return glm::distance(static_cast<glm::vec3>(*this), static_cast<glm::vec3>(other));
		}
		FVector Normalize()
		{
			return glm::normalize(static_cast<glm::vec3>(*this));
		}
		FVector Clamp(const FVector& min, const FVector& max) const
		{
			return glm::clamp(static_cast<glm::vec3>(*this), static_cast<glm::vec3>(min), static_cast<glm::vec3>(max));
		}

		operator glm::vec3() const { return glm::vec3(x, y, z); }
	};

} }