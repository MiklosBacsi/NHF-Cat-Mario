#ifndef SOUND_H
#define SOUND_H

#ifndef CPORTA

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

#include <iostream>
#include <vector>

#define MAX_CHANNELS 8

// SDL supports only 8 channels
// Channel 7 is reserved for playing background and lobby music
// Stoppnig looped channel is bugy, so I play empty audio on channel

class Sound {
public:
    enum Type {CLICK=0, BACKGROUND, LOBBY, DEATH, JUMP, SPAWN, BREAK, COIN, FISH, POP, FLAG, ERROR, EMPTY, COUNT=13};
private:
    std::vector<Mix_Chunk*> sounds;
    void FreeSound(Sound::Type soundType);
public:
    Sound();
    void LoadSound(const char* path, Sound::Type soundType);
    void PlaySound(Sound::Type soundType, bool loop = false);
    void StopSound();
    ~Sound();
};

#endif // CPORTA

#endif // SOUND_H