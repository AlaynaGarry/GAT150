#include "Timer.h"

namespace nc{
	Timer::clock_tick_type Timer::ElapsedTicks()
	{
		clockDuration duration = clock::now() - timePoint;
		return duration.count();
	}
	float Timer::ElapsedSeconds()
	{
		return ElapsedTicks() / static_cast<float>(clockDuration::period::den);
	}
	void FrameTimer::Tick()
	{
		clockDuration duration = clock::now() - startTimePoint;
		time = duration.count() / static_cast<float>(clockDuration::period::den);

		duration = clock::now() - frameTimePoint;
		deltaTime = (duration.count() / static_cast<float>(clockDuration::period::den)) * timeScale;

		frameTimePoint = clock::now();
	}
}