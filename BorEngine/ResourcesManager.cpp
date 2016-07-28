#include "ResourcesManager.h"


GLTexture ResourcesManager::getTexture(std::string texturePath)
{
	return _textureCache.getTexture(texturePath);
}
GLuint  ResourcesManager::boundTexture;
TextureCache ResourcesManager::_textureCache;