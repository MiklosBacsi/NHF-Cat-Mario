#ifndef CPORTA

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL2_gfxPrimitives.h>

#include <iostream>

#include "Entity.h"
#include "RigidBody.h"
#include "Texture.h"
#include "Animation.h" // ???

SDL_Texture* Entity::textures = nullptr;

/* ************************************************************************************ */

/***** Class Entity *****/
Entity::Entity(SDL_Rect hitBox, SDL_Rect srcRect, SDL_Rect destRect)
    : GameObject(hitBox, srcRect, destRect, textures),
        spawnPoint(hitBox), previousPosition(spawnPoint)
    {
    //
}

bool Entity::IsDead() const { return isRemoved; }

RigidBody& Entity::GetRigidBody() { return rigidBody; }

Entity::~Entity() {
    #ifdef DTOR
    std::clog << "~Entity Dtor" << std::endl;
    #endif
}

/* ************************************************************************************ */

/***** Class Player *****/
Player::Player(SDL_Rect hitBox, SDL_Rect srcRect, SDL_Rect destRect)
    : Entity(hitBox, srcRect, destRect),
        isForcedByFlag(false), isForcedByRobot(false)
    {
    //
}

void Player::Update(float dt) {
    rigidBody.Update(dt);
    hitBox.x += (int) rigidBody.GetPosition().x;
    hitBox.y += (int) rigidBody.GetPosition().y;
    
    texture.DestRect().x = hitBox.x;
    texture.DestRect().y = hitBox.y;
}

void Player::Render() {
    texture.Render();
    // HitBox:
    rectangleRGBA(window->GetRenderer(), hitBox.x, hitBox.y, hitBox.x + hitBox.w, hitBox.y + hitBox.h, 255, 0, 0, 255);
}

void Player::Reset() {
    rigidBody.Reset();
    hitBox = spawnPoint;
    previousPosition = spawnPoint;
    texture.DestRect().x = spawnPoint.x;
    texture.DestRect().y = spawnPoint.y;
    isRemoved = false;
    isForcedByFlag = false;
    isForcedByRobot = false;
}

void Player::Touch(GameObject* object) {}

void Player::TouchedBy(Entity* entity) {
    Touch(entity);
}

void Player::Kill() {
    if (isRemoved)
        return;

    isRemoved = true;
    --deathCount;
}

Player::~Player() {
    #ifdef DTOR
    std::clog << "~Player Dtor" << std::endl;
    #endif
}
/* ************************************************************************************ */

/***** Class Enemy *****/
Enemy::Enemy(SDL_Rect hitBox, float activationPoint, SDL_Rect srcRect, SDL_Rect destRect)
    : Entity(hitBox, srcRect, destRect), isActivated(false), hasQuote(false),
        activationPoint(activationPoint)
    {
    //
}

void Enemy::Update(float dt) {}

void Enemy::Render() {}

Enemy::~Enemy() {
    #ifdef DTOR
    std::clog << "~Enemy Dtor" << std::endl;
    #endif
}
#endif // CPORTA