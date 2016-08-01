#pragma once

#include "GLTexture.h"

#include <string>

namespace BorEngine
{
	class ImageLoader
	{
	public:
		static GLTexture loadPNG(std::string filePath);
	};
}