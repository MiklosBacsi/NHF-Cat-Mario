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
// Stopping looped channel is buggy, so I play empty audio on channel

/**
 * @brief Sound module.
 */
class Sound {
public:
    enum Type {CLICK=0, HAL, SAKANA, BACKGROUND, DEATH, JUMP, ROAR, BREAK, COIN, LASER, FISH, LOBBY, POP, FLAG, ERROR, EMPTY, COUNT=16};    ///< Type of sounds.
private:
    std::vector<Mix_Chunk*> sounds;     ///< Contains all the sounds.
    void FreeSound(Sound::Type soundType);  ///< Frees a sound by type.

public:
    /** 
     * @brief Default constructor.
     */
    Sound();

    /** 
     * @brief Loads a sound from path to a type.
     * @param path File path to the sound.
     * @param soundType Type of the sound.
     */
    void LoadSound(const char* path, Sound::Type soundType);

    /** 
     * @brief Plays a sound by type.
     * @param soundType Type of the sound.
     * @param loop Contains whether the sound is looped.
     */
    void PlaySound(Sound::Type soundType, bool loop = false);

    /** 
     * @brief Stops all sounds.
     */
    void StopSound();

    /** 
     * @brief Destructor.
     */
    ~Sound();
};

#endif // CPORTA

#endif // SOUND_H