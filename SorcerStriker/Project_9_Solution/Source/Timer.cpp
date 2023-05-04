#include "Timer.h"

Timer::Timer()
{
	currentTimerElapsed = 0;
	maxTime = 0;
	lastTime = SDL_GetTicks();
	deltaTimer = SDL_GetTicks() - lastTime;
}

void Timer::refreshTimer()
{
	deltaTimer = SDL_GetTicks() - lastTime;
	currentTimerElapsed += deltaTimer;
	lastTime = SDL_GetTicks();
}

void Timer::resetTimer()
{
	currentTimerElapsed = 0;
	deltaTimer = 0;
	lastTime = SDL_GetTicks();
}

bool Timer::hasCompleted()
{
	if (currentTimerElapsed >= maxTime) {
		return true;
	}
	return false;
}

