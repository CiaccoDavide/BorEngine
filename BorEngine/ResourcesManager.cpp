#include "ResourcesManager.h"

namespace BorEngine
{
	TextureCache ResourcesManager::_textureCache;

	GLTexture ResourcesManager::getTexture(std::string texturePath)
	{
		return _textureCache.getTexture(texturePath);
	}
}