#include "LuckyPch.h"
#include "ParticleSystem.h"

#include "Random.h"
#include "Lucky/Core/Renderer/Renderer2D.h"

namespace Lucky
{
	ParticleSystem::ParticleSystem(uint32_t maxParticles)
		: m_PoolIndex(maxParticles - 1)
	{
		m_ParticlePool.resize(maxParticles);
	}

	void ParticleSystem::OnUpdate(Timestep ts)
	{
		for (auto &particle : m_ParticlePool)
		{
			if (!particle.Active)
				continue;

			if (particle.LifeRemaining <= 0.0f)
			{
				particle.Active = false;
				continue;
			}

			particle.LifeRemaining -= ts;
			particle.Position += particle.Velocity * (float)ts;
			particle.Rotation += 0.01f * ts;
		}
	}

	void ParticleSystem::OnRender()
	{
		for (auto &particle : m_ParticlePool)
		{
			if (!particle.Active)
				continue;

			// Fade away particles
			float life = particle.LifeRemaining / particle.LifeTime;
			glm::vec4 color = glm::lerp(particle.ColorEnd, particle.ColorBegin, life);
			// color.a = color.a * life;

			float size = glm::lerp(particle.SizeEnd, particle.SizeBegin, life);
			Renderer2D::DrawRotateQuad({ particle.Position.x, particle.Position.y, 1.0f }, { size, size }, particle.Rotation, color);
		}
	}

	void ParticleSystem::Emit(const ParticleProps &particleProps)
	{
		Particle &particle = m_ParticlePool[m_PoolIndex];
		particle.Active = true;
		particle.Position = particleProps.Position;
		particle.Rotation = Random::Float() * 2.0f * glm::pi<float>();

		// Velocity
		particle.Velocity = particleProps.Velocity;
		particle.Velocity.x += particleProps.VelocityVariation.x * (Random::Float() - 0.5f);
		particle.Velocity.y += particleProps.VelocityVariation.y * (Random::Float() - 0.5f);

		// Color
		particle.ColorBegin = particleProps.ColorBegin;
		particle.ColorEnd = particleProps.ColorEnd;

		particle.LifeTime = particleProps.LifeTime;
		particle.LifeRemaining = particleProps.LifeTime;
		particle.SizeBegin = particleProps.SizeBegin + particleProps.SizeVariation * (Random::Float() - 0.5f);
		particle.SizeEnd = particleProps.SizeEnd;

		m_PoolIndex = --m_PoolIndex % m_ParticlePool.size();
	}
} // namespace Lucky