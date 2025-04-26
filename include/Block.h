#ifndef BLOCK_H
#define BLOCK_H

#ifndef CPORTA

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <iostream>

#include "Texture.h"

class Block {
public:
    enum Type { DIRT, STONE }; // etc.
    static SDL_Texture* textures;
private:
public:
    Block();
    // void update();
    void render(float cameraX);
    void activate();
    ~Block();
};


#endif // CPORTA

#endif // BLOCK_H