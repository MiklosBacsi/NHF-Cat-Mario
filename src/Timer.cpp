#include "SDL2/SDL.h"

#include "Timer.h"

Timer::Timer() : isActive(false) {}

Timer::Timer(size_t miliSeconds) : isActive(true), startTicks(SDL_GetTicks()), setDuration(miliSeconds) {}

void Timer::deactivate() { isActive = false; }

void Timer::activate(size_t miliSeconds) {
    isActive = true;
    setDuration = miliSeconds;
    startTicks = SDL_GetTicks();
}

bool Timer::getIsActive() const { return isActive; }

size_t Timer::getTime() const {
    if (!isActive)
        return 0;
    return SDL_GetTicks() - startTicks;
}

float Timer::getPercent() const {
    return getTime() / setDuration;
}