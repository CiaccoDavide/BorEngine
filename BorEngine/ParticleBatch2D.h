#pragma once

#include <functional>
#include <glm/glm.hpp>
#include "Vertex.h"
#include "SpriteBatch.h"
#include "GLTexture.h"

namespace BorEngine
{
	class Particle2D
	{
	public:
		glm::vec2 position = glm::vec2(0.0f);
		glm::vec2 velocity = glm::vec2(0.0f);
		ColorRGBA8 color;
		float lifeTime = 0.0f;
		float size = 0.0f;
	};
	inline	void defaultParticle2DUpdate(Particle2D& particle, float deltaTime)
	{
		particle.position += particle.velocity*deltaTime;
	}
	class ParticleBatch2D
	{

	public:
		//friend class Particle;

		ParticleBatch2D();
		~ParticleBatch2D();

		void init(
			int maxParticles,
			float decayRate,
			GLTexture texture,
			std::function<void(Particle2D&, float)> updateFunc = defaultParticle2DUpdate
		);
		void update(float deltaTime);
		void draw(SpriteBatch* spriteBatch);
		void addParticle(
			const glm::vec2& position,
			const glm::vec2& velocity,
			const ColorRGBA8& color,
			float size
		);

	private:
		int findFreeParticle();

		std::function<void(Particle2D&, float)> p_updateFunc;
		float p_decayRate;
		Particle2D* p_particles = nullptr;
		int p_maxParticles = 0;
		int p_lastFreeParticle = 0;
		GLTexture p_texture;
	};

}