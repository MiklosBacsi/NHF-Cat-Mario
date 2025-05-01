#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#ifndef CPORTA

#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"

#include <iostream>

#include "Texture.h"
#include "RigidBody.h"
#include "RenderWindow.h"

class GameObject {
public:
    static RenderWindow* window;
    static float CameraX;
protected:
    Texture texture;
    SDL_Rect hitBox;
    bool isRemoved;
public:
    GameObject(SDL_Rect hitBox, SDL_Rect srcRect, SDL_Rect destRect, SDL_Texture* texture);
    virtual void Update(float dt) = 0;
    virtual void Render() = 0;
    virtual void Reset() = 0;
    SDL_Rect& HitBox();
    virtual ~GameObject();
};

#endif // CPORTA

#endif // GAME_OBJECT_H