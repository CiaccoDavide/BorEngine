#include "Timing.h"
#include<SDL\SDL.h>

namespace BorEngine
{
	FPSLimiter::FPSLimiter()
	{

	}

	void FPSLimiter::init(float maxFPS)
	{
		setMaxFPS(maxFPS);
	}

	void FPSLimiter::begin()
	{
		p_startTicks = SDL_GetTicks();
	}

	float FPSLimiter::end()
	{
		float frameTicks = SDL_GetTicks() - p_startTicks;
		if (1000.0f / p_maxFPS > frameTicks)
		{
			SDL_Delay((Uint32)(1000.0f / p_maxFPS - frameTicks));
		}
		p_calculateFPS();
		return p_fps;
	}

	void FPSLimiter::setMaxFPS(float maxFPS)
	{
		p_maxFPS = maxFPS;
	}

	void FPSLimiter::p_calculateFPS()
	{
		static const int NUM_SAMPLES = 10;
		static float frameTimes[NUM_SAMPLES];
		static int currentFrame = 0;
		static float prevTicks = (float)SDL_GetTicks();
		float currentTicks;
		currentTicks = (float)SDL_GetTicks();

		p_frameTime = currentTicks - prevTicks;

		frameTimes[currentFrame%NUM_SAMPLES] = p_frameTime;

		prevTicks = currentTicks;

		int count;

		currentFrame++;

		if (currentFrame < NUM_SAMPLES)
		{
			count = currentFrame;
		}
		else
		{
			count = NUM_SAMPLES;
		}
		float frameTimeAverage = 0;
		for (int i = 0; i < count; i++)
		{
			frameTimeAverage += frameTimes[i];
		}
		frameTimeAverage /= count;

		if (frameTimeAverage > 0)
		{
			p_fps = 1000.0f / frameTimeAverage;
		}
		else
		{
			p_fps = 0.0f;
		}
	}
}