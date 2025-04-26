#ifndef ANIMATION_H
#define ANIMATION_H

#ifndef CPORTA

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <iostream>

#include "Texture.h"

class Animation {
private:
public:
    Animation();
    void render(float cameraX);
    ~Animation();
};




#endif // CPORTA

#endif // ANIMATION_H