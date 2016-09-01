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
		void _calculateFPS();

		unsigned int _startTicks;
		float _fps;
		float _maxFPS;
		float _frameTime;
	};
}