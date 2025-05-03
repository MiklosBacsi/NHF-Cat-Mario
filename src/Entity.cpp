#ifndef CPORTA

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL2_gfxPrimitives.h>

#include <iostream>

#include "Entity.h"
#include "RigidBody.h"
#include "Texture.h"

SDL_Texture* Entity::textures = nullptr;

/* ************************************************************************************ */

/***** Class Entity *****/
Entity::Entity(SDL_Rect hitBox, SDL_Rect srcRect, SDL_Rect destRect, bool faceLeft)
    : GameObject(hitBox, srcRect, destRect, textures), spawnPoint(hitBox),
        previousPosition(spawnPoint), recoverX(false), recoverY(false), faceLeft(faceLeft)
    {
    //
}

void Entity::LimitedBy(GameObject* object) {
    hasCollided = true;
    int right=0, left=0, up=0, down=0;

    // First: Fix Vertical Collision
    up = GameObject::OverhangUp(hitBox, object->HitBox());
    down = GameObject::OverhangDown(hitBox, object->HitBox());

    if (up > 0 && down > 0) {
        recoverX = true; // !!!!!!!!!!!!!!!!!!!!!4
        hasCollided = false;
    }
    else if (up > 0) {
        hitBox.y += up;
        rigidBody.ApplyVelocityY(0.0f);
    }
    else if (down > 0) {
        hitBox.y -= (down - 2);
        rigidBody.ApplyVelocityY(0.0f);
        rigidBody.ApplyForceY(RigidBody::gravity * -1.0f);
        if (Player* player = dynamic_cast<Player*>(this))
            player->OnGround() = true;
    }

    // Then: Fix Horizontal Collision
    if (object->HitBox().y + 1 < hitBox.y + hitBox.h && hitBox.y + 1 < object->HitBox().y + object->HitBox().h)
        right = GameObject::OverhangRight(hitBox, object->HitBox());
    if (object->HitBox().y + 1 < hitBox.y + hitBox.h && hitBox.y + 1 < object->HitBox().y + object->HitBox().h)
        left = GameObject::OverhangLeft(hitBox, object->HitBox());

    if (right > 0 && left > 0)
        recoverX = true;
    else if (right > 0) {
        hitBox.x -= right;
        rigidBody.ApplyVelocityX(0.0f);
    }
    else if (left > 0) {
        hitBox.x += left;
        rigidBody.ApplyVelocityX(0.0f);
    }
    #ifdef COLLISION
    std::clog << "*****\n";
    if (right > 0) std::clog << "Collision: Right\t" << right << std::endl;
    if (left > 0) std::clog << "Collision: Left\t" << left << std::endl;
    if (up > 0) std::clog << "Collision: Up\t" << up << std::endl;
    if (down > 0) std::clog << "Collision: Down\t" << down << std::endl;
    #endif
}

bool Entity::IsDead() const { return isRemoved; }

RigidBody& Entity::GetRigidBody() { return rigidBody; }

void Entity::UpdatePreviousPosition() { previousPosition = hitBox; }

Entity::~Entity() {
    if (Entity::textures == nullptr) {
        SDL_DestroyTexture(Entity::textures);
        Entity::textures = nullptr;
    }
    #ifdef DTOR
    std::clog << "~Entity Dtor" << std::endl;
    #endif
}
/* ************************************************************************************ */

/***** Class Player *****/
Player::Player(SDL_Rect hitBox, SDL_Rect srcRect, SDL_Rect destRect)
    : Entity(hitBox, srcRect, destRect, false), deathCount(3), isForcedByFlag(false),
        isForcedByRobot(false), onGround(false), jump(false), runSprite(false), runTime(0.0f)
    {
    //
}

void Player::Update(float dt) {
    onGround = false;
    hasCollided = false;
    rigidBody.Update(dt);
    hitBox.x += (int) rigidBody.GetPosition().x;
    if (hitBox.x - GameObject::screen.x < 5)
        hitBox.x = GameObject::screen.x + 5;
    if (hitBox.x + hitBox.w >= GameObject::screen.x + GameObject::screen.w - 5)
        hitBox.x = GameObject::screen.x + GameObject::screen.w - hitBox.w - 5;
    hitBox.y += (int) rigidBody.GetPosition().y;
    
    texture.DestRect().x = hitBox.x;
    texture.DestRect().y = hitBox.y;
}

void Player::Render() {
    // Chosing correct Sprite
    if (faceLeft)
        texture.SrcRect().y = 34;
    else
        texture.SrcRect().y = 0;
    
    if (isRemoved)
        texture.SrcRect().x = 72;
    else if (!onGround)
        texture.SrcRect().x = 48;
    else if (runSprite)
        texture.SrcRect().x = 24;
    else
        texture.SrcRect().x = 0;
        
    texture.Render();
    // HitBox:
    //rectangleRGBA(window->GetRenderer(), hitBox.x, hitBox.y, hitBox.x + hitBox.w, hitBox.y + hitBox.h, 255, 0, 0, 255);
    // Dest Rect:
    #ifdef COLLISION
    rectangleRGBA(window->GetRenderer(), texture.DestRect().x, texture.DestRect().y, texture.DestRect().x + texture.DestRect().w, texture.DestRect().y + texture.DestRect().h, 0, 0, 255, 255);
    #endif
    // Previous Position:
    // rectangleRGBA(window->GetRenderer(), previousPosition.x, previousPosition.y, previousPosition.x + previousPosition.w, previousPosition.y + previousPosition.h, 0, 255, 0, 255);
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
    recoverX = false;
    recoverY = false;
    texture.SrcRect() = {0, 0, 24, 34};
}

void Player::Touch(GameObject* object) {
    object->TouchedBy(this);
}

void Player::TouchedBy(Entity* entity) {
    Touch(entity);
}

void Player::Kill() {
    if (isRemoved)
        return;

    isRemoved = true;
    --deathCount;
}

bool& Player::OnGround() { return onGround; }

Player::~Player() {
    #ifdef DTOR
    std::clog << "~Player Dtor" << std::endl;
    #endif
}
/* ************************************************************************************ */

/***** Class Enemy *****/
Enemy::Enemy(SDL_Rect hitBox, float activationPoint, SDL_Rect srcRect, SDL_Rect destRect, bool faceLeft)
    : Entity(hitBox, srcRect, destRect, faceLeft), isActivated(false), hasQuote(false),
        activationPoint(activationPoint)
    {
    //
}

void Enemy::Update(float dt) {}

void Enemy::Render() {}

void Enemy::Reset() {}

void Enemy::Touch(GameObject* object) {}

void Enemy::TouchedBy(Entity* entity) {
    if (dynamic_cast<Enemy*>(entity))
        return;
    if (Player* player = dynamic_cast<Player*>(entity)) {
        //
        player->GetRigidBody();
        return;
    }
    throw "Unknown entity!";
}

Enemy::~Enemy() {
    #ifdef DTOR
    std::clog << "~Enemy Dtor" << std::endl;
    #endif
}
#endif // CPORTA