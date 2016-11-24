#pragma once

#include "GLTexture.h"

#include <GL/glew.h>
#include <string>

namespace BorEngine
{
	class Sprite
	{
	public:
		Sprite();
		~Sprite();

		void init(float x, float y, float width, float height, std::string texturePath);

		void draw();

	private:
		float p_x;
		float p_y;
		float p_width;
		float p_height;

		GLuint p_vboID; // vertex buffer object ID

		GLTexture p_texture;
	};
}
