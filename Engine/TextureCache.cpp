#include "TextureCache.h"

#include <iostream>

#include "ImageLoader.h"

namespace Engine
{
	TextureCache::TextureCache(void)
	{
	}


	TextureCache::~TextureCache(void)
	{
	}

	Engine::GLTexture TextureCache::GetTexture(const char* texturePath)
	{
		//lookup for texture and see if its there
		auto mit = textureMap.find(texturePath);

		//check if its not in the map
		if(mit == textureMap.end())
		{
			//load the texture
			GLTexture newTexture = ImageLoader::LoadSoil(texturePath);

			//insert it into the map
			textureMap.insert(std::make_pair(texturePath, newTexture));

			//std::cout<<"Load Texture!\n";
			return newTexture;
		}
		//std::cout<<"Used Cached Texture!\n";
		return mit->second;
	}
}
