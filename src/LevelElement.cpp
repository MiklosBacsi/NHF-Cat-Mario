#ifndef CPORTA

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <iostream>

#include "LevelElement.h"
#include "Texture.h"
#include "RigidBody.h"

SDL_Texture* LevelElement::textures = nullptr;

LevelElement::LevelElement() {}

LevelElement::~LevelElement() {
    #ifdef DTOR
    std::clog << "~LevelElement Dtor" << std::endl;
    #endif
}



#endif // CPORTA