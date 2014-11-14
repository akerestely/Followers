#pragma once
#include "TextureCache.h"

namespace Engine
{
	class ResourceMngr
	{
	public:
		static GLTexture GetTexture(char* texturePath);
	private:
		static TextureCache textureCache;
	};
}

