#pragma once

namespace BorEngine
{
	class FPSLimiter
	{
	public:
		FPSLimiter();
		void init(float maxFPS);

		void begin();
		float end(); // this will return the current fps

		void setMaxFPS(float maxFPS);
	private:
		void p_calculateFPS();

		unsigned int p_startTicks;
		float p_fps;
		float p_maxFPS;
		float p_frameTime;
	};
}