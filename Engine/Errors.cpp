#include "Errors.h"
#include <iostream>
#include <sdl/SDL.h>

namespace Engine
{
	void fatalError(std::string msg)
	{
		std::cout<<msg<<'\n';
		system("pause");
		SDL_Quit();
		exit(1);
	}
}
