#pragma once

#include "TextureCache.h"
#include <string>

namespace BorEngine
{
	class ResourcesManager
	{
	public:
		static GLTexture getTexture(std::string texturePath);
	private:
		static TextureCache _textureCache;
	};
}
