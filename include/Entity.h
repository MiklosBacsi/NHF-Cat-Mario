#ifndef ENTITY_H
#define ENTITY_H

#ifndef CPORTA

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <iostream>

#include "RigidBody.h"
#include "Animation.h"
#include "Texture.h"

class Entity {
public:
    static SDL_Texture* textures;
protected:
    Vector2D transform;
    Vector2D dimension;
    RigidBody rigidBody;
    Vector2D spawnPoint;
    bool isDead;
public:
    Entity();
    virtual void update(float dt) = 0;
    virtual void render(float cameraX) = 0;
    virtual ~Entity();
};

class Player : public Entity {
private:
public:
    Player();
    void update(float dt);
    void render(float cameraX);
    ~Player();
};

class Enemy : public Entity {
private:
    bool isActivated;
    float activationPoint;
public:
    Enemy();
    void update(float dt);
    void render(float cameraX);
    ~Enemy();
};

#endif // CPORTA

#endif // ENTITY_H