#pragma once

#include "Particle.h"
#include "Sand/Core/Time.h"
#include "Sand/Core/Random.h"
#include "Sand/Utils/Gradient.h"

namespace Sand
{

	class ParticleEmitter
	{
	public:
		struct Properties
		{
			// Positional
			glm::vec2 Position{ 0.0f };
			glm::vec2 Velocity{ 0.0f }, VelocityVariation{ 1.0f };
			// Color
			ColorGradient ColorOverLifetime;
			// Scale
			glm::vec2 ScaleBegin{ 1.0f }, ScaleEnd{ 0.0f };
			float ScaleVariation = 0.0f;
			// Rotation
			float StartRotation = 0.0f;
			float RotationVariation = 1.0f;
			// Life
			float Lifetime = 1.0f;
			float MaxEmissionTime = 5.0f;
			
			float EmissionTime = 0.0f;
		};
	public:
		ParticleEmitter();

		void Play();
		void Stop();

		void OnUpdate(Timestep ts);
		void OnRender(Timestep ts);

		Properties& GetProperties() { return m_Props; }
		void ResetProperties()
		{
			m_Props.Position = { 0.0f, 0.0f };
			m_Props.Velocity = { 0.0f, 0.0f };
			m_Props.VelocityVariation = { 1.0f, 1.0f };
			m_Props.ColorOverLifetime.GetMarks().clear();
			m_Props.ScaleBegin = { 1.0f, 1.0f };
			m_Props.ScaleEnd = { 0.0f, 0.0f };
			m_Props.ScaleVariation = 0.0f;
			m_Props.StartRotation = 0.0f;
			m_Props.RotationVariation = 1.0f;
			m_Props.Lifetime = 1.0f;
		}
	private:
		void UpdateParticle();
	private:
		Properties m_Props;
		bool m_Playing = false;

		std::vector<Particle> m_Particles;
		uint32_t m_ParticleIndex = 999;
	};

}