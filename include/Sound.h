#ifndef SOUND_H
#define SOUND_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

#include <iostream>

class Sound {
public:
    enum Type {BUTTON=0, BACKGROUND, DEATH, JUMP, SPAWN, BREAK, COIN, FISH, POP, FLAG};
private:
    Mix_Music* sound;
public:
    Sound(const char* path);
    void playSound(bool loop = false);
    void stopSound();
    ~Sound();
};

#endif // SOUND_H