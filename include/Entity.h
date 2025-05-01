#ifndef ENTITY_H
#define ENTITY_H

#ifndef CPORTA

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <iostream>

#include "GameObject.h"
#include "Texture.h"
#include "RigidBody.h"

class Entity : public GameObject {
protected:
    static SDL_Texture* textures;
    RigidBody rigidBody;
    Vector2D spawnPoint;
    Vector2D previousPosition;
    bool isDead;
public:
    Entity();
    virtual void Update(float dt) = 0;
    virtual void Render() = 0;
    virtual void Reset() = 0;
    virtual void Touch(GameObject* object) = 0;
    virtual void TouchedBy(Entity* entity) = 0;
    virtual ~Entity();
};

class Player : public Entity {
private:
    bool isForcedByFlag;
    bool isForcedByRobot;
public:
    Player();
    void Update(float dt);
    void Render();
    void Reset();
    void Touch(GameObject* object);
    void TouchedBy(Entity* entity);
    ~Player();
};

class Enemy : public Entity {
protected:
    bool isActivated;
    float activationPoint;
    struct Quote quote;
public:
    Enemy();
    virtual void Update(float dt) = 0;
    virtual void Render() = 0;
    virtual void Reset() = 0;
    virtual void Touch(GameObject* object) = 0;
    virtual void TouchedBy(Entity* entity) = 0;
    ~Enemy();
};

#endif // CPORTA

#endif // ENTITY_H