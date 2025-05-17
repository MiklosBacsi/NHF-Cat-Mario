/** @file Sound.cpp
  * @brief Contains the implementation of Sound.h
  * @author Bácsi Miklós
  * @date 2025-05-18
*/

#ifndef CPORTA

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

#include <iostream>

#include "Sound.h"

Sound::Sound() : sounds( Sound::COUNT, nullptr) {
    if ((Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024)) == -1) {
        std::cerr << "Audio Library not working!" << Mix_GetError() << std::endl;
        exit(1);
    }
    Mix_AllocateChannels(MAX_CHANNELS);
}

void Sound::LoadSound(const char* path, Sound::Type soundType) {
    if (soundType == Sound::COUNT)
        throw "Wrong type!";
    
    FreeSound(soundType);
    
    sounds[soundType] = Mix_LoadWAV(path);
    if (sounds[soundType] == nullptr)
        throw "Failed to load Audio";
}

void Sound::PlaySound(Sound::Type soundType, bool loop) {
    if (soundType == COUNT)
        throw "Wrong type!";
    if (sounds[soundType] == nullptr)
        throw "Sound not loaded in!";

    int Loop = 0;
    if (loop)
        Loop = -1;
    
    int channel = soundType % (MAX_CHANNELS - 1);
    if (soundType == Sound::BACKGROUND || soundType == Sound::LOBBY)
        channel = MAX_CHANNELS - 1;

    Mix_PlayChannel(channel, sounds[soundType], Loop);
}

void Sound::StopSound() {
    Mix_HaltMusic();
    Mix_PlayChannel(MAX_CHANNELS - 1, sounds[EMPTY], 0);
}

void Sound::FreeSound(Sound::Type soundType) {
    if (sounds[soundType] != nullptr) {
        Mix_FreeChunk(sounds[soundType]);
        sounds[soundType] = nullptr;
    }
}

Sound::~Sound() {
    for (Mix_Chunk* sound : sounds)
        if (sound != nullptr)
            Mix_FreeChunk(sound);

    #ifdef DTOR
    std::clog << "~Sound Dtor" << std::endl;
    #endif
}

#endif // CPORTA