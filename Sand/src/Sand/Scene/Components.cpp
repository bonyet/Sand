#include "sandpch.h"
#include "Components.h"

#include "Sand/Scripting/ScriptEngine.h"

namespace Sand
{

	// BOX COLLIDER
	void BoxColliderComponent::Create(const glm::vec2& scale, b2Body* const body)
	{
		Body = body;
		m_Scale = scale;

		// Shape stuff
		b2PolygonShape shape;
		shape.SetAsBox((scale.x / 2.0f) - shape.m_radius, (scale.y / 2.0f) - shape.m_radius);
		// Fixture stuff
		b2FixtureDef fixtureDef;
		fixtureDef.shape = &shape;
		fixtureDef.density = 1.0f;
		fixtureDef.friction = m_Friction;
		fixtureDef.restitution = m_Restitution;

		Fixture = body->CreateFixture(&fixtureDef);
		reinterpret_cast<b2Fixture*>(Fixture)->SetSensor(m_Observer);
	}

	void BoxColliderComponent::Destroy()
	{
		if (!Body)
			return;

		Body->DestroyFixture(Body->GetFixtureList());
	}

	void BoxColliderComponent::SetScale(const glm::vec2& scale)
	{
		m_Scale = scale;

		if (Body)
			reinterpret_cast<b2PolygonShape*>(Body->GetFixtureList()->GetShape())->SetAsBox(scale.x / 2.0f, scale.y / 2.0f);
	}

	glm::vec2 BoxColliderComponent::GetScale() const
	{
		return m_Scale;
	}

	void BoxColliderComponent::SetObserver(bool flag)
	{
		m_Observer = flag;

		if (Body)
			Body->GetFixtureList()->SetSensor(flag);
	}

	bool BoxColliderComponent::IsObserver() const
	{
		return m_Observer;
	}

	void BoxColliderComponent::SetRestitution(float restitution)
	{
		m_Restitution = restitution;

		if (Body)
			Body->GetFixtureList()->SetRestitution(m_Restitution);
	}

	float BoxColliderComponent::GetRestitution() const
	{
		return m_Restitution;
	}

	void BoxColliderComponent::SetFriction(float friction)
	{
		m_Friction = friction;

		if (Body)
			Body->GetFixtureList()->SetFriction(friction);
	}

	float BoxColliderComponent::GetFriction() const
	{
		return m_Friction;
	}

	// SCRIPT
	void ScriptComponent::Init()
	{
		if (!ScriptEngine::IsModuleRegistered((uint32_t)owner))
			ScriptEngine::RegisterModule((uint32_t)owner, ModuleName);
	}

	void ScriptComponent::Uninit()
	{
		ScriptEngine::UnregisterModule((uint32_t)owner);
	}

}