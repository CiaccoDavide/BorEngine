#include "TextureCache.h"
#include "ImageLoader.h"
#include <iostream>

namespace BorEngine
{
	TextureCache::TextureCache()
	{
	}

	TextureCache::~TextureCache()
	{
	}

	GLTexture TextureCache::getTexture(std::string texturePath)
	{
		// std::map<std::string,GLTexture>::iterator mit =	_textureMap.find(texturePath); // mit stands for "Map Iterator"
		auto mit = p_textureMap.find(texturePath); // inizializzando si puo' usare "auto" per ricevere automaticamente il tipo

		if (mit == p_textureMap.end()) // not found
		{
			GLTexture newTexture = ImageLoader::loadPNG(texturePath);

			// std::pair<std::string, GLTexture> newPair(texturePath, newTexture);
			// _textureMap.insert(newPair);
			p_textureMap.insert(make_pair(texturePath, newTexture));

			std::cout << "Loading a texture! [NEW]\n";

			return newTexture;
		}

		std::cout << "Loading a texture! [CACHED]\n";

		return mit->second;
	}
}