#pragma once

#include <map>
#include "GLTexture.h"

namespace BorEngine
{
	class TextureCache
	{
	public:
		TextureCache();
		~TextureCache();

		GLTexture getTexture(std::string texturePath);

	private:
		std::map<std::string, GLTexture> p_textureMap;
	};
}
