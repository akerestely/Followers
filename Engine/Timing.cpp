#include "Timing.h"

#include <SDl/SDL.h>

namespace Engine
{
	FpsLimiter::FpsLimiter()
	{

	}

	void FpsLimiter::Init(float maxFps)
	{
		SetMaxFps( maxFps);
	}


	void FpsLimiter::SetMaxFps(float maxFps)
	{
		this->maxFps = maxFps;
	}

	void FpsLimiter::Begin()
	{
		startTicks = SDL_GetTicks();
	}

	float FpsLimiter::End()
	{
		calculateFps();

		float frameTicks = SDL_GetTicks() - startTicks;
		if(1000.0f / maxFps > frameTicks)
			SDL_Delay(1000.0f / maxFps - frameTicks);

		return fps;
	}

	void FpsLimiter::calculateFps()
	{
		static const int NUM_SAMPLES = 10;
		static float frameTimes[NUM_SAMPLES];
		static int currentFrame = 0;

		static float prevTicks = SDL_GetTicks();

		float currentTicks;
		currentTicks = SDL_GetTicks();

		frameTime = currentTicks - prevTicks;
		frameTimes[currentFrame % NUM_SAMPLES] = frameTime;

		prevTicks = currentTicks;

		int count;
		currentFrame++;
		if(currentFrame < NUM_SAMPLES)
			count = currentFrame;
		else
			count = NUM_SAMPLES;

		float frameTimeAverage = 0;
		for(int i=0; i<count; i++)
			frameTimeAverage += frameTimes[i];
		frameTimeAverage /= count;

		if(frameTimeAverage>0)
			fps = 1000.0f / frameTimeAverage;
		else
			fps = 0;
	}

}