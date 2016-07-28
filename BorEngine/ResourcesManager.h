#pragma once

#include "TextureCache.h"
#include <string>

class ResourcesManager
{
public:
	static GLTexture getTexture(std::string texturePath);
	static GLuint boundTexture;
private:
	static TextureCache _textureCache;
};

