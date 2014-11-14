#include "Errors.h"
#include <iostream>
#include <sdl/SDL.h>

namespace Engine
{
	void fatalError(char* msg)
	{
		std::cout<<msg<<'\n';
		system("pause");
		SDL_Quit();
		exit(1);
	}
}
