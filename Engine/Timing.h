#pragma once

namespace Engine
{
	class FpsLimiter
	{
	public:
		FpsLimiter();
		void Init(float targetFps);

		void SetMaxFps(float targetFps);

		void Begin();

		//will return current fps
		float End();
	private:
		void calculateFps();

		float fps;
		float maxFps;
		float frameTime;
		unsigned int startTicks;
	};
}