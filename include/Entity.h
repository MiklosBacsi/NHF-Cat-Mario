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
    bool playSound;
public:
    Entity(SDL_Rect hitBox, SDL_Rect srcRect, SDL_Rect destRect, bool faceLeft=true);
    virtual void Update(float dt) = 0;
    virtual void Render() = 0;
    virtual void Reset() = 0;
    virtual void Touch(GameObject* object) = 0;
    virtual void TouchedBy(Entity* entity) = 0; // For interacting with other entities
    virtual void LimitedBy(GameObject* object); // For handling collision
    bool IsDead() const;
    bool HasCollided() const;
    RigidBody& GetRigidBody();
    virtual void UpdatePreviousPosition();
    virtual void Kill() = 0;
    virtual ~Entity();
};

class Player : public Entity {
    friend class GameEngine;
public:
    static int position;
    static int GetPosition();   // Necessary for comparison with Enemy's activationPoint
private:
    int deathCount;
    bool isGiga;
    bool isForcedByFlag;
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
    void MakeGiga();
    bool& OnGround();
    ~Player();
};

class Enemy : public Entity {
    friend class GameEngine;
protected:
    bool isActivated;
    int activationPoint;
    int shiftTextureRight;
public:
    Enemy(SDL_Rect hitBox, int activationPoint, SDL_Rect srcRect, int shiftTextureRight, SDL_Rect destRect, bool faceLeft);
    virtual void Update(float dt);
    virtual void Render();
    virtual void Reset();
    virtual void Touch(GameObject* object);
    virtual void TouchedBy(Entity* entity) = 0;
    virtual void Kill();
    ~Enemy();
};

class CommonEnemy : public Enemy {
public:
    CommonEnemy(SDL_Rect hitBox, int activationPoint, SDL_Rect srcRect, int shiftTextureRight, SDL_Rect destRect, bool faceLeft);
    void TouchedBy(Entity* entity);
    ~CommonEnemy();
};

class SoldierEnemy : public Enemy {
private:
    bool isSqueezed;
    bool isSliding;
public:
    SoldierEnemy(SDL_Rect hitBox, int activationPoint, SDL_Rect srcRect, int shiftTextureRight, SDL_Rect destRect, bool faceLeft);
    void Update(float dt) override;
    void Reset() override;
    void TouchedBy(Entity* entity);
    bool IsSqueezed() const;
    ~SoldierEnemy();
};

class KingEnemy : public Enemy {
public:
    KingEnemy(SDL_Rect hitBox, int activationPoint, SDL_Rect srcRect, int shiftTextureRight, SDL_Rect destRect, bool faceLeft);
    void TouchedBy(Entity* entity);
    ~KingEnemy();
};

class RedMushroomEnemy : public Enemy {
    public:
        RedMushroomEnemy(SDL_Rect hitBox, int activationPoint, SDL_Rect srcRect, int shiftTextureRight, SDL_Rect destRect, bool faceLeft);
        void TouchedBy(Entity* entity);
        ~RedMushroomEnemy();
    };

class PurpleMushroomEnemy : public Enemy {
public:
    PurpleMushroomEnemy(SDL_Rect hitBox, int activationPoint, SDL_Rect srcRect, int shiftTextureRight, SDL_Rect destRect, bool faceLeft);
    void TouchedBy(Entity* entity);
    ~PurpleMushroomEnemy();
};

#endif // CPORTA

#endif // ENTITY_H