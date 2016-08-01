
#include <SDL\SDL.h>
#include <GL\glew.h>

#include "BorEngine.h"

namespace BorEngine
{
	int init()
	{
		SDL_Init(SDL_INIT_EVERYTHING);

		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1); // against flickering!

		return 0;
	}
}