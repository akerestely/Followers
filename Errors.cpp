#include "Errors.h"
#include <iostream>
#include <sdl/SDL.h>

void fatalError(char* msg)
{
	std::cout<<msg<<'\n';
	system("pause");
	SDL_Quit();
}
