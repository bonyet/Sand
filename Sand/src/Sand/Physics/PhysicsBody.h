#pragma once

#include <glm/glm.hpp>
#include <box2d/box2d.h>

namespace Sand
{

	struct PhysicsBodyDescription
	{
		glm::vec2 Position;
		glm::vec2 Scale;
		float Rotation;
	};

	enum class PhysicsBodyType
	{
		Static = b2_staticBody, Kinematic = b2_kinematicBody, Dynamic = b2_dynamicBody
	};

	class PhysicsBody
	{
	public:
		PhysicsBody();
		~PhysicsBody();

		void Create(const PhysicsBodyDescription& description, class PhysicsWorld* const physicsWorld);
		void Destroy();

		void SetType(PhysicsBodyType type);
		PhysicsBodyType GetType() const { return m_Type; }
		
		void SetMass(float mass);
		float GetMass() const;

		void SetGravityScale(float gravity);
		float GetGravityScale() const;

		void SetFriction(float friction);
		float GetFriction() const;

		void SetFixedRotation(bool rotation);
		bool GetFixedRotation() const;

		glm::vec2 GetPosition();
		float GetRotation();

		void ApplyLinearImpulse(const glm::vec2& impulse, bool wake = true);
		void ApplyForce(const glm::vec2& force, bool wake = true);
	private:
		b2Body* m_Body = nullptr;

		float m_Mass = 1.0f, m_Friction = 0.3f, m_GravityScale = 1.0f;
		bool m_FixedRotation = false;

		PhysicsBodyType m_Type = PhysicsBodyType::Static;
	};

}