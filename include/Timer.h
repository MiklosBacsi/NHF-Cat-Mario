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
    void Deactivate();
    void Activate(size_t miliSeconds);
    size_t GetTime() const;
    bool IsActive() const;
    bool HasExpired() const;
    float GetPercent() const;
    ~Timer();
};

#endif // CPORTA

#endif // TIMER_H