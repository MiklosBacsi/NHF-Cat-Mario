#ifndef CPORTA

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <iostream>

#include "GameObject.h"
#include "Texture.h"
#include "RigidBody.h"

RenderWindow* GameObject::window = nullptr;
float GameObject::CameraX = 0.0f;

GameObject::GameObject(SDL_Rect hitBox, SDL_Rect srcRect, SDL_Rect destRect, SDL_Texture* texture)
    : texture(texture, srcRect, destRect, true), hitBox(hitBox), isRemoved(false)
    {
    //
}

SDL_Rect& GameObject::HitBox() { return hitBox; }

GameObject::~GameObject() {
    #ifdef DTOR
    std::clog << "~GameObject Dtor" << std::endl;
    #endif
}

#endif // CPORTA