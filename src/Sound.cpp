#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <iostream>

#include "Sound.h"

Sound::Sound(const char* path) : sound(nullptr) {
    if ((Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024)) == -1) {
        std::cout << "Audio Library not working!" << Mix_GetError() << std::endl;
        exit(1);
    }

    sound = Mix_LoadMUS(path);
    if (sound == nullptr)
        throw "Failed to load Audio";
}

void Sound::playSound(bool loop) {
    int Loop = 0;
    if (loop)
        Loop = -1;
    Mix_PlayMusic(sound, Loop);
}

void Sound::stopSound() {
    Mix_PauseMusic();
}

Sound::~Sound() {
    Mix_FreeMusic(sound);
}