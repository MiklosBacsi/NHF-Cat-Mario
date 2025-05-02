#ifndef CPORTA

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <iostream>

#include "GameObject.h"
#include "Texture.h"
#include "RigidBody.h"

RenderWindow* GameObject::window = nullptr;
SDL_Rect GameObject::screen = {0,0,0,0};
/* ************************************************************************************ */

/***** Static Functions *****/
bool GameObject::AABB(const SDL_Rect& A, const SDL_Rect& B) {
    if (A.x + A.w >= B.x && A.x <= B.x + B.w && A.y + A.h >= B.y && A.y <= B.y + B.h)
        return true;
    return false;
}

int GameObject::OverhangRight(const SDL_Rect& A, const SDL_Rect& B) {
    if (A.x + A.w >= B.x + B.w)
        return 0;
    return A.x + A.w - B.x;
}

int GameObject::OverhangLeft(const SDL_Rect& A, const SDL_Rect& B) {
    if (A.x <= B.x)
        return 0;
    return B.x + B.w - A.x;
}

int GameObject::OverhangUp(const SDL_Rect& A, const SDL_Rect& B) {
    if (A.y <= B.y)
        return 0;
    return B.y + B.h - A.y;
}

int GameObject::OverhangDown(const SDL_Rect& A, const SDL_Rect& B) {
    if (A.y + A.h >= B.y + B.h)
        return 0;
    return A.y + A.h - B.y;
}
/* ************************************************************************************ */

/***** Class GameObject *****/
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