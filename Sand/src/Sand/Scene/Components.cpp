#include "sandpch.h"
#include "Components.h"

namespace Sand
{

	void PhysicsComponent::Init()
	{
	}

	void PhysicsComponent::SimulateStep(PhysicsWorld* const world, Timestep timestep)
	{
		glm::vec2 force = world->GetGravity();

		mLastAcceleration = mAcceleration;

		// Move
		auto& position = owner.GetComponent<TransformComponent>().Position;
		position += mLinearVelocity * (float)timestep + (0.5f * mLastAcceleration * (timestep * timestep));
		// Find acceleration
		mAcceleration = force / Mass;
		auto avgAcceleration = (mLastAcceleration + mAcceleration) / 2.0f;
		mLinearVelocity += avgAcceleration * (float)timestep;
	}

}