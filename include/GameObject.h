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
    Vector2D position;
    Vector2D hitBox;
public:
    GameObject();
    virtual void Update(float dt) = 0;
    virtual void Render() = 0;
    virtual void Reset() = 0;
    Vector2D& Position();
    const Vector2D& HitBox();
    virtual ~GameObject();
};

#endif // CPORTA

#endif // GAME_OBJECT_H