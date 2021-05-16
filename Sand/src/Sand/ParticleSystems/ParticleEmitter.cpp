#include "sandpch.h"

#include "ParticleEmitter.h"
#include "Sand/Renderer/Renderer2D.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/compatibility.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace Sand
{

	static int RandomInt()
	{
		static std::mt19937 engine;
		static std::uniform_int_distribution<std::mt19937::result_type> distribution;

		return distribution(engine) % 2;
	}

	static float RandomFloat()
	{
		static std::mt19937 engine;
		static std::uniform_int_distribution<std::mt19937::result_type> distribution;

		return static_cast<float>(distribution(engine)) / static_cast<float>(std::numeric_limits<uint32_t>::max());
	}

	ParticleEmitter::ParticleEmitter()
	{
		m_Particles.resize(static_cast<size_t>(m_ParticleIndex) + 1);
	}

	void ParticleEmitter::UpdateParticle()
	{
		Particle& particle = m_Particles[m_ParticleIndex];

		particle.Alive = true;

		// Set fields of particle
		particle.Position = m_Props.Position;

		particle.Rotation = glm::radians(RandomFloat());
		particle.RotateClockwise = static_cast<bool>(RandomInt());

		particle.Velocity = m_Props.Velocity;
		particle.Velocity.x += m_Props.VelocityVariation.x * (RandomFloat() - 0.5f);
		particle.Velocity.y += m_Props.VelocityVariation.y * (RandomFloat() - 0.5f);

		particle.Lifetime = m_Props.Lifetime;
		particle.LifeRemaining = m_Props.Lifetime;
		particle.ScaleBegin = m_Props.ScaleBegin + m_Props.ScaleVariation * (RandomFloat() - 0.5f);

		particle.ScaleEnd = m_Props.ScaleEnd;

		m_ParticleIndex = --m_ParticleIndex % m_Particles.size();
	}

	void ParticleEmitter::OnUpdate(Timestep ts)
	{
		if (!m_Playing)
			return;

		m_Props.EmissionTime += ts;
		if (m_Props.EmissionTime >= m_Props.MaxEmissionTime)
		{
			Stop();
		}

		UpdateParticle();
		
		for (Particle& particle : m_Particles)
		{
			if (particle.LifeRemaining <= 0.0f)
			{
				particle.Alive = false;
				continue;
			}

			particle.LifeRemaining -= ts;
			particle.Position += particle.Velocity * static_cast<float>(ts);

			particle.Rotation += particle.RotateClockwise ? ts : -ts;
		}
	}

	void ParticleEmitter::Play()
	{
		m_Playing = true;
	}
	void ParticleEmitter::Stop()
	{
		m_Props.EmissionTime = 0.0f;
		m_Playing = false;
	}

	void ParticleEmitter::OnRender(Timestep ts)
	{
		if (!m_Playing)
			return;

		for (Particle& particle : m_Particles)
		{
			if (!particle.Alive)
				continue;

			// Fade away
			float life = particle.LifeRemaining / particle.Lifetime;

			glm::vec4 particleColor{ 1.0f };
			m_Props.ColorOverLifetime.Evaluate(1.0f - life, &particleColor.x);

			glm::vec2 size = glm::lerp(particle.ScaleEnd, particle.ScaleBegin, life);

			// Render
			glm::mat4 transform = glm::translate(glm::mat4(1.0f), glm::vec3(particle.Position, 0.0f))
				* glm::rotate(glm::mat4(1.0f), particle.Rotation,  { 0, 0, 1 })
				* glm::scale(glm::mat4(1.0f), glm::vec3(size, 1.0f));

			Renderer2D::DrawQuad(transform, particleColor);
		}
	}

}