#pragma once
#include <map>

#include "GLTexture.h"

namespace Engine
{
	class TextureCache
	{
	public:
		TextureCache(void);
		~TextureCache(void);

		GLTexture GetTexture(const char* texturePath);
	private:
		std::map<const char*, GLTexture> textureMap;
	};
}

