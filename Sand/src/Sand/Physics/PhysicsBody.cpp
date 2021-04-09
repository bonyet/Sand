#include "sandpch.h"
#include "PhysicsBody.h"

#include "PhysicsWorld.h"

namespace Sand
{

	PhysicsBody::PhysicsBody()
	{
	}
	
	PhysicsBody::~PhysicsBody()
	{
		m_Body = nullptr;
	}
	
	void PhysicsBody::Create(const PhysicsBodyDescription& description, PhysicsWorld* const physicsWorld)
	{
		// Body stuff
		b2BodyDef bodyDef;
		bodyDef.type = (b2BodyType)m_Type;
		bodyDef.position.Set(description.Position.x, description.Position.y);
		bodyDef.angle = description.Rotation;
		bodyDef.gravityScale = m_GravityScale;
		bodyDef.fixedRotation = m_FixedRotation;

		m_Body = physicsWorld->GetWorld()->CreateBody(&bodyDef);

		b2MassData massData;
		m_Body->GetMassData(&massData);
		massData.mass = m_Mass;
		m_Body->SetMassData(&massData);
	}
	
	void PhysicsBody::Destroy()
	{
		m_Body = nullptr;
	}

	void PhysicsBody::SetType(PhysicsBodyType type)
	{
		m_Type = type;

		if (m_Body)
			m_Body->SetType((b2BodyType)type);
	}

	void PhysicsBody::SetMass(float mass)
	{
		m_Mass = mass;

		b2MassData data;
		data.mass = mass;

		if (m_Body)
			m_Body->SetMassData(&data);
	}

	float PhysicsBody::GetMass() const
	{
		return m_Mass;
	}

	void PhysicsBody::SetGravityScale(float gravity)
	{
		m_GravityScale = gravity;

		if (m_Body)
			m_Body->SetGravityScale(gravity);
	}

	float PhysicsBody::GetGravityScale() const
	{
		return m_GravityScale;
	}

	void PhysicsBody::SetFixedRotation(bool rotation)
	{
		m_FixedRotation = rotation;

		if (m_Body)
			m_Body->SetFixedRotation(rotation);
	}

	bool PhysicsBody::GetFixedRotation() const
	{
		return m_FixedRotation;
	}

	glm::vec2 PhysicsBody::GetPosition()
	{
		return { m_Body->GetPosition().x, m_Body->GetPosition().y };
	}

	float PhysicsBody::GetRotation()
	{
		return m_Body->GetAngle();
	}

	void PhysicsBody::ApplyLinearImpulse(const glm::vec2& impulse, bool wake)
	{
		m_Body->ApplyLinearImpulseToCenter({ impulse.x, impulse.y }, wake);
	}

	void PhysicsBody::ApplyForce(const glm::vec2& force, bool wake)
	{
		m_Body->ApplyForceToCenter({ force.x, force.y }, wake);
	}

}
