/** @file Timer.h
  * @brief Helps measuring time with a timer that can be activated for given amount of time.
  * @author Bácsi Miklós
  * @date 2025-05-18
*/

#ifndef TIMER_H
#define TIMER_H

#ifndef CPORTA

#include "SDL2/SDL.h"

#include <iostream>

/**
 * @brief Timer module.
 */
class Timer {
private:
    bool isActive;      ///< Contains whether timer is active.
    Uint32 startTicks;  ///< Contains the start of the timer.
    size_t setDuration; ///< Contains the set duration of the timer.

public:
    /**
     * @brief Default constructor.
     */
    Timer();

    /**
     * @brief Constructor that sets the timer to a given duration.
     * @param milliSeconds Duration given in miliseconds.alignas
     */
    Timer(size_t milliSeconds);

    /**
     * @brief Deactivates the timer.
     */
    void Deactivate();

    /**
     * @brief Activates the timer for a given amount of time.
     * @param milliSeconds Duration time given in miliseconds.
     */
    void Activate(size_t milliSeconds);

    /**
     * @return Returns the elapsed time in miliseconds.
     */
    size_t GetTime() const;

    /**
     * @return Return whether time is active.
     */
    bool IsActive() const;

    /**
     * @return Returns whether the timer has expired.
     */
    bool HasExpired() const;

    /**
     * @return Returns the completed percentage of the given session.
     */
    float GetPercent() const;

    /**
     * @brief Destructor.
     */
    ~Timer();
};

#endif // CPORTA

#endif // TIMER_H