#include "ParticleBatch2D.h"

namespace BorEngine
{
	void Particle2D::update(float deltaTime)
	{
		p_position += p_velocity*deltaTime;
	}

	ParticleBatch2D::ParticleBatch2D()
	{
	}


	ParticleBatch2D::~ParticleBatch2D()
	{
		delete[] p_particles;
	}


	void ParticleBatch2D::init(int maxParticles, float decayRate, GLTexture texture)
	{
		p_maxParticles = maxParticles;
		p_particles = new Particle2D[maxParticles];
		p_decayRate = decayRate;
		p_texture = texture;
	}

	void ParticleBatch2D::update(float deltaTime)
	{
		for (int i = 0; i < p_maxParticles; i++)
		{
			if (p_particles[i].p_lifeTime > 0.0f)
			{
				p_particles[i].update(deltaTime);
				p_particles[i].p_lifeTime -= p_decayRate*deltaTime;
			}
		}
	}

	void ParticleBatch2D::draw(SpriteBatch* spriteBatch)
	{
		glm::vec4 uvRect(0.0f, 0.0f, 1.0f, 1.0f);
		for (int i = 0; i < p_maxParticles; i++)
		{
			if (p_particles[i].p_lifeTime > 0.0f)
			{
				glm::vec4 destRect(p_particles[i].p_position.x, p_particles[i].p_position.y, p_particles[i].p_size, p_particles[i].p_size);
				spriteBatch->draw(destRect, uvRect, p_texture.id, 0.0f, p_particles[i].p_color);
			}
		}
	}

	void ParticleBatch2D::addParticle(const glm::vec2& position, const glm::vec2& velocity, const ColorRGBA8& color, float size)
	{
		int particleIndex = findFreeParticle();

		p_particles[particleIndex].p_lifeTime = 1.0f;
		p_particles[particleIndex].p_position = position;
		p_particles[particleIndex].p_velocity = velocity;
		p_particles[particleIndex].p_color = color;
		p_particles[particleIndex].p_size = size;
	}

	int ParticleBatch2D::findFreeParticle()
	{
		for (int i = p_lastFreeParticle; i < p_maxParticles; i++)
		{
			if (p_particles[i].p_lifeTime <= 0.0f)
			{
				p_lastFreeParticle = i;
				return i;
			}
		}

		for (int i = 0; i < p_maxParticles; i++)
		{
			if (p_particles[i].p_lifeTime <= 0.0f)
			{
				p_lastFreeParticle = i;
				return i;
			}
		}

		return 0;
	}
}