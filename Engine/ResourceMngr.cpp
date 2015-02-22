#include "ResourceMngr.h"

namespace Engine
{
	TextureCache ResourceMngr::textureCache;

	Engine::GLTexture ResourceMngr::GetTexture(const char* texturePath)
	{
		return textureCache.GetTexture(texturePath);
	}
}