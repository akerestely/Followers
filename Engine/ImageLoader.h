#pragma once

#include "GLTexture.h"

namespace Engine
{
	class ImageLoader
	{
	public:
		static GLTexture LoadPNG(char* filepPath);
		static GLTexture LoadSoil(const char* filePath);
	};
}

