#ifndef TIMER_H
#define TIMER_H

#include "SDL2/SDL.h"

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
};

#endif // TIMER_H