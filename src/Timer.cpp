/** @file Timer.cpp
  * @brief Contains the implementation of Timer.h
  * @author Bácsi Miklós
  * @date 2025-05-18
*/

#ifndef CPORTA

#include "SDL2/SDL.h"

#include <iostream>

#include "Timer.h"

Timer::Timer() : isActive(false), setDuration(1) {}

Timer::Timer(size_t milliSeconds) : isActive(true), startTicks(SDL_GetTicks()), setDuration(milliSeconds) {}

void Timer::Deactivate() { isActive = false; }

void Timer::Activate(size_t milliSeconds) {
    isActive = true;
    setDuration = milliSeconds;
    startTicks = SDL_GetTicks();
}

size_t Timer::GetTime() const {
    if (!isActive)
        return 0;
    return SDL_GetTicks() - startTicks;
}

bool Timer::IsActive() const { return isActive; }

bool Timer::HasExpired() const { return GetPercent() > 1.0f; }

float Timer::GetPercent() const {
    return (float) GetTime() / (float) setDuration;
}

Timer::~Timer() {
    #ifdef DTOR
    std::clog << "~Timer Dtor" << std::endl;
    #endif
}

#endif // CPORTA