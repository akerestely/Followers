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

		GLTexture GetTexture(char* texturePath);
	private:
		std::map<char*, GLTexture> textureMap;
	};
}

