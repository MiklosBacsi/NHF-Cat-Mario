#ifndef CPORTA

#include "SDL2/SDL.h"

#include <iostream>

#include "Timer.h"

Timer::Timer() : isActive(false), setDuration(1) {}

Timer::Timer(size_t miliSeconds) : isActive(true), startTicks(SDL_GetTicks()), setDuration(miliSeconds) {}

void Timer::deactivate() { isActive = false; }

void Timer::activate(size_t miliSeconds) {
    isActive = true;
    setDuration = miliSeconds;
    startTicks = SDL_GetTicks();
}

size_t Timer::getTime() const {
    if (!isActive)
        return 0;
    return SDL_GetTicks() - startTicks;
}

bool Timer::getIsActive() const { return isActive; }

bool Timer::hasExpired() const { return getPercent() > 1.0f; }

float Timer::getPercent() const {
    return (float) getTime() / (float) setDuration;
}

Timer::~Timer() {
    #ifdef DTOR
    std::clog << "~Timer Dtor" << std::endl;
    #endif
}

#endif // CPORTA