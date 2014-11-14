#pragma once
#include "TextureCache.h"

class ResourceMngr
{
public:
	static GLTexture GetTexture(char* texturePath);
private:
	static TextureCache textureCache;
};

