#pragma once
#include "TextureCache.h"

#include <string>

namespace Engine
{
	class ResourceMngr
	{
	public:
		static GLTexture GetTexture(const char* texturePath);
	private:
		static TextureCache textureCache;
	};
}

