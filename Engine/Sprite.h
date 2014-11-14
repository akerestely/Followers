#pragma once
#include <gl\glew.h>

#include "GLTexture.h"
#include "ResourceMngr.h"

namespace Engine
{
	class Sprite
	{
	public:
		Sprite(void);
		void Init(float x, float y, float width, float height, char* texturePath);
		void Draw();
		~Sprite(void);
	private:
		GLuint vboId;
		float x,y,width,height;

		GLTexture texture;
	};
}

