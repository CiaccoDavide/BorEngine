#pragma once

#include <glm/glm.hpp>
#include "Vertex.h"
#include "SpriteBatch.h"
#include "GLTexture.h"

namespace BorEngine
{
	class Particle2D
	{
	public:
		friend class ParticleBatch2D;
		void update(float deltaTime);
	private:
		glm::vec2 p_position = glm::vec2(0.0f);
		glm::vec2 p_velocity = glm::vec2(0.0f);
		ColorRGBA8 p_color;
		float p_lifeTime = 0.0f;
		float p_size = 0.0f;
	};

	class ParticleBatch2D
	{
	public:
		//friend class Particle;

		ParticleBatch2D();
		~ParticleBatch2D();

		void init(int maxParticles,float decayRate,GLTexture texture);
		void update(float deltaTime);
		void draw(SpriteBatch* spriteBatch);
		void addParticle(const glm::vec2& position, const glm::vec2& velocity, const ColorRGBA8& color, float size);

	private:
		int findFreeParticle();

		float p_decayRate;
		Particle2D* p_particles = nullptr;
		int p_maxParticles = 0;
		int p_lastFreeParticle = 0;
		GLTexture p_texture;
	};

}