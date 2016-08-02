#include "Window.h"
#include "Errors.h"

namespace BorEngine
{
	Window::Window()
	{
	}

	Window::~Window()
	{
	}

	int Window::create(std::string windowName, int screenWidth, int screenHeight, unsigned int flags)
	{
		Uint32 windowFlags = SDL_WINDOW_OPENGL;

		if (flags & MINIMIZED)
		{
			windowFlags |= SDL_WINDOW_MINIMIZED;
		}
		if (flags & FULLSCREEN)
		{
			windowFlags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
		}
		if (flags & BORDERLESS)
		{
			windowFlags |= SDL_WINDOW_BORDERLESS;
		}

		_sdlWindow = SDL_CreateWindow(windowName.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, screenWidth, screenHeight, windowFlags);

		if (_sdlWindow == nullptr)
		{
			fatalError("SDL window could not be created!");
		}

		// we need an openGL context
		SDL_GLContext glContext = SDL_GL_CreateContext(_sdlWindow);
		if (glContext == nullptr)
		{
			fatalError("SDL_GL context could not be created!");
		}

		if (glewInit() != GLEW_OK)
		{
			fatalError("Could not initialize Glew!");
		}


		std::printf("\n ### OpenGL Version: %s ###\n\n", glGetString(GL_VERSION));

		glClearColor(0.569f, 0.204f, 0.192f, 1.0f); // set the background color, will be applied whenever GL_COLOR_BUFFER_BIT will be called

		SDL_GL_SetSwapInterval(0); // turn on VSYNC



		return 0;
	}

	void Window::swapBuffer()
	{
		SDL_GL_SwapWindow(_sdlWindow);
	}
}