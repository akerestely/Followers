#include "ResourceMngr.h"

namespace Engine
{
	TextureCache ResourceMngr::textureCache;

	GLTexture ResourceMngr::GetTexture(char* texturePath)
	{
		return textureCache.GetTexture(texturePath);
	}
}