#include "ImageLoader.h"

#include "picoPNG.h"
#include "IOManager.h"
#include "Errors.h"

#include <Windows.h>
#include <soil/SOIL.h>

namespace Engine
{
	GLTexture ImageLoader::LoadPNG(char* filePath)
	{
		GLTexture texture = {};

		std::vector<unsigned char> in;
		std::vector<unsigned char> out;
		unsigned long width, height;

		if(!IOManager::ReadFileToBuffer(filePath, in))
			fatalError("Failed to load PNG file to buffer!");

		int errorCode = decodePNG(out, width, height, &(in[0]), in.size());
		if (errorCode)
			fatalError("Decode PNG failed!");

		glGenTextures(1, &(texture.id));

		glBindTexture(GL_TEXTURE_2D, texture.id);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

		//for OpenGl <3.0
		glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);
		//for OpenGl >3.0
		//glGenerateMipmap(GL_TEXTURE_2D);
	
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, &(out[0]));

		glBindTexture(GL_TEXTURE_2D, 0);

		texture.width=width;
		texture.height=height;

		return texture;
	}

	Engine::GLTexture ImageLoader::LoadSoil(char* filePath)
	{
		GLTexture texture = {};
		//load image with SOIL
		int nChannels;
		unsigned char* buffer = SOIL_load_image(filePath, &texture.width, &texture.height, &nChannels, SOIL_LOAD_RGBA);
		if(!buffer)
		{
			std::string errorMsg(SOIL_last_result());
			fatalError("SOIL failed at '" + std::string(filePath) + "': " + errorMsg);
		}

		//generate OpenGL texture
		glGenTextures(1, &(texture.id));

		glBindTexture(GL_TEXTURE_2D, texture.id);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

		//for OpenGl <3.0
		glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);
		//for OpenGl >3.0
		//glGenerateMipmap(GL_TEXTURE_2D);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texture.width, texture.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, buffer);

		return texture;
	}
}