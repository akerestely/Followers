#pragma once

#include "GLTexture.h"

#include <map>
#include <string>

namespace Engine
{
	class TextureCache
	{
	public:
		TextureCache(void);
		~TextureCache(void);

		GLTexture GetTexture(const char* texturePath);
	private:
		std::map<std::string, GLTexture> textureMap;
	};
}

