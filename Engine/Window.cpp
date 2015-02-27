#include "Window.h"

#include "Errors.h"
#include <stdio.h>

namespace Engine
{
	Window::Window(void)
	{
	}


	Window::~Window(void)
	{
	}

	void Window::Create(char* windowTitle, int screenWidth, int screenHeight, unsigned int currentFlags)
	{
		Uint32 flags = SDL_WINDOW_OPENGL;
		if (currentFlags & INVISIBLE)
			flags |= SDL_WINDOW_HIDDEN;
		if (currentFlags & FULLSCREEN)
			flags |= SDL_WINDOW_FULLSCREEN;
		if (currentFlags & BORDERLESS)
			flags |= SDL_WINDOW_BORDERLESS;

		SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
		SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
		SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
		SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
		SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
		SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);

		sdlWindow = SDL_CreateWindow(windowTitle, SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED, screenWidth, screenHeight, flags);

		if(sdlWindow==nullptr)
			fatalError("SDL window could not be loaded");

		SDL_GLContext glContex = SDL_GL_CreateContext(sdlWindow);
		if(glContex == nullptr)
			fatalError("SQL_GL context could not be created");

		GLenum glewError=glewInit();
		if(glewError != GLEW_OK)
			fatalError("Could not initialize glew!");

		printf("***   OpenGl Version: %s   ***\n", glGetString(GL_VERSION));

		glClearColor( 1.0f, 1.0f, 1.0f, 1.0f );

		//set v-sync
		SDL_GL_SetSwapInterval(0);

		//
		//glEnable(GL_BLEND);
		//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}

	void Window::SwappBuffer()
	{
		SDL_GL_SwapWindow(sdlWindow);
	}

	bool Window::IsFullscreen()
	{
		return SDL_GetWindowFlags(sdlWindow) & SDL_WINDOW_FULLSCREEN;
	}

	void Window::Fullscreen(bool visibility)
	{
		if(visibility)
			SDL_SetWindowFullscreen(sdlWindow,SDL_WINDOW_FULLSCREEN);
		else
			SDL_SetWindowFullscreen(sdlWindow,0);
	}

}
