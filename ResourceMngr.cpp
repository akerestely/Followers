#include "ResourceMngr.h"

TextureCache ResourceMngr::textureCache;

GLTexture ResourceMngr::GetTexture(char* texturePath)
{
	return textureCache.GetTexture(texturePath);
}