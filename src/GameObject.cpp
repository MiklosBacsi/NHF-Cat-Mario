#ifndef CPORTA

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <iostream>

#include "GameObject.h"
#include "Texture.h"
#include "RigidBody.h"

RenderWindow* GameObject::window = nullptr;
float GameObject::CameraX = 0.0f;

GameObject::GameObject() {}

Vector2D& GameObject::Position() { return position; }

const Vector2D& GameObject::HitBox() { return hitBox; }

GameObject::~GameObject() {
    #ifdef DTOR
    std::clog << "~GameObject Dtor" << std::endl;
    #endif
}

#endif // CPORTA