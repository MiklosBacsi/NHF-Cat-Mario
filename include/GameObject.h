#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#ifndef CPORTA

#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"

#include <iostream>

#include "Texture.h"
#include "RigidBody.h"
#include "RenderWindow.h"

// Forward declaration
class Entity;
class GameEngine;

class GameObject {
public:
    static RenderWindow* window;
    static SDL_Rect screen;
    static bool AABB(const SDL_Rect& A, const SDL_Rect& B);
    static int OverhangRight(const SDL_Rect& A, const SDL_Rect& B); // A: Entity, B: Barrier
    static int OverhangLeft(const SDL_Rect& A, const SDL_Rect& B);
    static int OverhangUp(const SDL_Rect& A, const SDL_Rect& B);
    static int OverhangDown(const SDL_Rect& A, const SDL_Rect& B);
    static bool JumpedOnHead(const SDL_Rect& player, const SDL_Rect& enemy);
protected:
    Texture texture;
    SDL_Rect hitBox;
    bool isRemoved;
public:
    GameObject(SDL_Rect hitBox, SDL_Rect srcRect, SDL_Rect destRect, SDL_Texture* texture, bool isRemoved=false);
    virtual void Update(float dt) = 0;
    virtual void Render() = 0;
    virtual void Reset() = 0;
    virtual void TouchedBy(Entity* entity) = 0;
    virtual SDL_Rect& HitBox();
    virtual void UpdateDestRect();
    virtual ~GameObject();
};

#endif // CPORTA

#endif // GAME_OBJECT_H