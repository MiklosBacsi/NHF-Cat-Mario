#ifndef SOUND_H
#define SOUND_H

#ifndef CPORTA

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

#include <iostream>
#include <vector>

// SDL supports only 8 channels
// Channel 7 is reserved for playing background and lobby music
// Stoppnig looped channel is bugy, so I play empty audio on channel

class Sound {
public:
    enum Type {CLICK=0, BACKGROUND, LOBBY, DEATH, JUMP, SPAWN, BREAK, COIN, FISH, POP, FLAG, EMPTY, COUNT=12};
private:
    std::vector<Mix_Chunk*> sounds;
    void freeSound(Sound::Type soundType);
public:
    Sound();
    void loadSound(const char* path, Sound::Type soundType);
    void playSound(Sound::Type soundType, bool loop = false);
    void stopSound();
    ~Sound();
};

#endif // CPORTA

#endif // SOUND_H