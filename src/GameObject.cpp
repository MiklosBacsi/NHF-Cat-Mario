#ifndef CPORTA

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <iostream>

#include "GameObject.h"
#include "Texture.h"
#include "RigidBody.h"

RenderWindow* GameObject::window = nullptr;
SDL_Rect GameObject::screen = {0,0,0,0};

GameObject::GameObject(SDL_Rect hitBox, SDL_Rect srcRect, SDL_Rect destRect, SDL_Texture* texture)
    : texture(texture, srcRect, destRect, true), hitBox(hitBox), isRemoved(false)
    {
    screen = {0, 0, window->GetWidth()-1, window->GetHeight()-1};
}

SDL_Rect& GameObject::HitBox() { return hitBox; }

void GameObject::UpdateDestRect() {
    texture.DestRect().x = hitBox.x - screen.x;
    texture.DestRect().y = hitBox.y - screen.y;
}

GameObject::~GameObject() {
    #ifdef DTOR
    std::clog << "~GameObject Dtor" << std::endl;
    #endif
}

#endif // CPORTA