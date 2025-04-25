#ifndef TIMER_H
#define TIMER_H

#ifndef CPORTA

#include "SDL2/SDL.h"

#include <iostream>

class Timer {
private:
    bool isActive;
    Uint32 startTicks;
    size_t setDuration;
public:
    Timer();
    Timer(size_t miliSeconds);
    void deactivate();
    void activate(size_t miliSeconds);
    size_t getTime() const;
    bool getIsActive() const;
    bool hasExpired() const;
    float getPercent() const;
    ~Timer();
};

#endif // CPORTA

#endif // TIMER_H