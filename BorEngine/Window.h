#pragma once

#include <SDL\SDL.h>
#include <GL\glew.h>
#include <string>

namespace BorEngine
{
	enum WindowFlags
	{
		DEFAULT = 0,
		MINIMIZED = 0x1, // when the game starts minimized the sprites are not rendered...why?
		FULLSCREEN = 0x2,
		BORDERLESS = 0x4
	};

	class Window
	{
	public:
		Window();
		~Window();

		int create(std::string windowName, int screenWidth, int screenHeight, unsigned int flags);

		void swapBuffer();

		int getScreenWidth() { return _screenWidth; }
		int getScreenHeight() { return _screenHeight; }

	private:
		SDL_Window* _sdlWindow;
		int _screenWidth, _screenHeight;
	};
}
