#pragma once
#include <gl/glew.h>
#include <sdl/SDL.h>

namespace Engine
{
	enum WindowFlags {INVISIBLE = 0x1, FULLSCREEN = 0x2, BORDERLESS = 0x4};

	class Window
	{
	public:
		Window(void);
		~Window(void);

		void Create(char* windowTitle, int screenWidth, int screenHeight, unsigned int currentFlags);

		void SwappBuffer();

		int GetScreenWidth() { return screenWidth; }
		int GetScreenHeight() { return screenHeight; }

		bool IsFullscreen();
		void Fullscreen(bool visibility);
	private:
		SDL_Window *sdlWindow;
		int screenWidth, screenHeight;
	};
}

