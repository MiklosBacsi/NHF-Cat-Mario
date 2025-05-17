/** @file GameObject.cpp
  * @brief Contains the implementation of GameObject.h
  * @author Bácsi Miklós
  * @date 2025-05-18
*/

#ifndef CPORTA

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <iostream>

#include "GameObject.h"
#include "Texture.h"
#include "RigidBody.h"
#include "Entity.h"

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
    SDL_Rect middle = {A.x + (A.w/2), A.y - 5, 1, 1};
    if (AABB(middle, B) == false)
        return 0;
    return B.y + B.h - A.y;
}

int GameObject::OverhangDown(const SDL_Rect& A, const SDL_Rect& B) {
    if (A.y + A.h >= B.y + B.h || AABB(A,B) == false)
        return 0;
    int overHang = A.y + A.h - B.y + 1;
    if (overHang > 20) {
        #ifdef COLLISION
        std::clog << "Critical Overhang: Down" << std::endl;
        #endif
        return 0;
    }
    SDL_Rect middle = {A.x + (A.w/2), A.y + A.h + 5, 1, 1};
    if (AABB(middle, B) == false)
        return 0;
    return overHang;
}

bool GameObject::JumpedOnHead(const SDL_Rect& player, const SDL_Rect& enemy) {
    if (player.y + player.h >= enemy.y + enemy.h || AABB(player,enemy) == false)
        return false;
    int overHang = player.y + player.h - enemy.y + 1;
    if (overHang > 20)
        return false;
    return true;
}
/* ************************************************************************************ */

/***** Class GameObject *****/
GameObject::GameObject(SDL_Rect hitBox, SDL_Rect srcRect, SDL_Rect destRect, SDL_Texture* texture, bool isRemoved)
    : texture(texture, srcRect, destRect, true), hitBox(hitBox), isRemoved(isRemoved)
    {
    //
}

SDL_Rect& GameObject::HitBox() { return hitBox; }

void GameObject::UpdateDestRect() {
    texture.DestRect().x = hitBox.x - screen.x;
    texture.DestRect().y = hitBox.y - screen.y;

    if (dynamic_cast<Entity*>(this)) {
        texture.DestRect().y -= 1;
    }
}

GameObject::~GameObject() {
    #ifdef DTOR
    std::clog << "~GameObject Dtor" << std::endl;
    #endif
}

#endif // CPORTA