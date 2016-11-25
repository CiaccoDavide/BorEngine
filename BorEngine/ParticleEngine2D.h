#pragma once

#include <vector>

namespace BorEngine 
{
	class ParticleBatch2D; // forward declaration (we'll include it in the cpp file), compile times will be better!
	class SpriteBatch;

	class ParticleEngine2D
	{
	public:
		ParticleEngine2D();
		~ParticleEngine2D();

		void addParticleBatch(ParticleBatch2D* particleBatch);
		void update(float deltaTime);
		void draw(SpriteBatch* spriteBatch);

	private:
		std::vector<ParticleBatch2D*> p_batches;
	};

}