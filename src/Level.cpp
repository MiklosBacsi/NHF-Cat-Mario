#ifndef CPORTA

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <iostream>

#include "Level.h"
#include "Entity.h"
#include "RigidBody.h"
#include "Animation.h"
#include "Texture.h"
#include "Block.h"
#include "Element.h"

Level::Level(const char* path) {}


Level::~Level() {
    #ifdef DTOR
    std::clog << "~Level Dtor" << endl;
    #endif
}

#endif // CPORTA