#include "ParticleBatch2D.h"

namespace BorEngine
{
	

	ParticleBatch2D::ParticleBatch2D()
	{
	}


	ParticleBatch2D::~ParticleBatch2D()
	{
		delete[] p_particles;
	}


	void ParticleBatch2D::init(int maxParticles, float decayRate, GLTexture texture, std::function<void(Particle2D&, float)> updateFunc)
	{
		p_maxParticles = maxParticles;
		p_particles = new Particle2D[maxParticles];
		p_decayRate = decayRate;
		p_texture = texture;
		p_updateFunc = updateFunc;
	}

	void ParticleBatch2D::update(float deltaTime)
	{
		for (int i = 0; i < p_maxParticles; i++)
		{
			if (p_particles[i].lifeTime > 0.0f)
			{
				p_updateFunc(p_particles[i],deltaTime);
				p_particles[i].lifeTime -= p_decayRate*deltaTime;
			}
		}
	}

	void ParticleBatch2D::draw(SpriteBatch* spriteBatch)
	{
		glm::vec4 uvRect(0.0f, 0.0f, 1.0f, 1.0f);
		for (int i = 0; i < p_maxParticles; i++)
		{
			if (p_particles[i].lifeTime > 0.0f)
			{
				glm::vec4 destRect(p_particles[i].position.x, p_particles[i].position.y, p_particles[i].size, p_particles[i].size);
				spriteBatch->draw(destRect, uvRect, p_texture.id, 0.0f, p_particles[i].color);
			}
		}
	}

	void ParticleBatch2D::addParticle(const glm::vec2& position, const glm::vec2& velocity, const ColorRGBA8& color, float size)
	{
		int particleIndex = findFreeParticle();

		p_particles[particleIndex].lifeTime = 1.0f;
		p_particles[particleIndex].position = position;
		p_particles[particleIndex].velocity = velocity;
		p_particles[particleIndex].color = color;
		p_particles[particleIndex].size = size;
	}

	int ParticleBatch2D::findFreeParticle()
	{
		for (int i = p_lastFreeParticle; i < p_maxParticles; i++)
		{
			if (p_particles[i].lifeTime <= 0.0f)
			{
				p_lastFreeParticle = i;
				return i;
			}
		}

		for (int i = 0; i < p_maxParticles; i++)
		{
			if (p_particles[i].lifeTime <= 0.0f)
			{
				p_lastFreeParticle = i;
				return i;
			}
		}

		return 0;
	}
}