#ifndef ENTITY_H
#define ENTITY_H

#ifndef CPORTA

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <iostream>

#include "GameObject.h"
#include "Texture.h"
#include "RigidBody.h"
#include "Timer.h"

class Entity : public GameObject {
    friend class GameEngine;
public:
    static SDL_Texture* textures;
protected:
    RigidBody rigidBody;
    SDL_Rect spawnPoint;
    SDL_Rect previousPosition;
    bool hasCollided;
    bool recoverX;
    bool recoverY;
    bool faceLeft;
public:
    Entity(SDL_Rect hitBox, SDL_Rect srcRect, SDL_Rect destRect, bool faceLeft=true);
    virtual void Update(float dt) = 0;
    virtual void Render() = 0;
    virtual void Reset() = 0;
    virtual void Touch(GameObject* object) = 0;
    virtual void TouchedBy(Entity* entity) = 0; // For interacting with other entities
    virtual void LimitedBy(GameObject* object); // For handling collision
    bool IsDead() const;
    RigidBody& GetRigidBody();
    virtual void UpdatePreviousPosition();
    virtual ~Entity();
};

class Player : public Entity {
    friend class GameEngine;
private:
    int deathCount;
    bool isForcedByFlag;
    bool isForcedByRobot;
    bool onGround;
    bool jump;
    Timer jumpTime;
    bool runSprite;
    float runTime;
public:
    Player(SDL_Rect hitBox, SDL_Rect srcRect, SDL_Rect destRect);
    void Update(float dt);
    void Render();
    void Reset();
    void Touch(GameObject* object);
    void TouchedBy(Entity* entity);
    void Kill();
    bool& OnGround();
    ~Player();
};

class Enemy : public Entity {
protected:
    bool isActivated;
    bool hasQuote;
    float activationPoint;
    struct Quote quote;
public:
    Enemy(SDL_Rect hitBox, float activationPoint, SDL_Rect srcRect, SDL_Rect destRect, bool faceLeft);
    virtual void Update(float dt) = 0;
    virtual void Render() = 0;
    virtual void Reset() = 0;
    virtual void Touch(GameObject* object);
    virtual void TouchedBy(Entity* entity);
    ~Enemy();
};

#endif // CPORTA

#endif // ENTITY_H