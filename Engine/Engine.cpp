#include "Engine.h"

#include <sdl/SDL.h>
#undef main

namespace Engine
{
	void Init()
	{
		SDL_Init(SDL_INIT_EVERYTHING);

		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	}
}