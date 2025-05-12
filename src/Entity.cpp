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
        previousPosition(spawnPoint), recoverX(false), recoverY(false), faceLeft(faceLeft), playSound(false)
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

        if (dynamic_cast<Enemy*>(this))
            faceLeft = true;
    }
    else if (left > 0) {
        hitBox.x += left;
        rigidBody.ApplyVelocityX(0.0f);

        if (dynamic_cast<Enemy*>(this))
            faceLeft = false;
    }

    if (hasCollided && (dynamic_cast<Enemy*>(this))) {
        rigidBody.ApplyForceY(0.0f);
    }

    #ifdef COLLISION
    if (dynamic_cast<Player*>(this)) {
        std::clog << "*****\n";
        if (right > 0) std::clog << "Collision: Right\t" << right << std::endl;
        if (left > 0) std::clog << "Collision: Left\t" << left << std::endl;
        if (up > 0) std::clog << "Collision: Up\t" << up << std::endl;
        if (down > 0) std::clog << "Collision: Down\t" << down << std::endl;
        rigidBody.Velocity().Log();
        rigidBody.Acceleration().Log();
    }
    #endif
}

bool Entity::IsDead() const { return isRemoved; }

bool Entity::HasCollided() const { return hasCollided; }

RigidBody& Entity::GetRigidBody() { return rigidBody; }

void Entity::UpdatePreviousPosition() { previousPosition = hitBox; }

Entity::~Entity() {
    #ifdef DTOR
    std::clog << "~Entity Dtor" << std::endl;
    #endif
}
/* ************************************************************************************ */

/***** Class Player *****/
int Player::position = 0;

Player::Player(SDL_Rect hitBox, SDL_Rect srcRect, SDL_Rect destRect)
    : Entity(hitBox, srcRect, destRect, false), deathCount(3), isGiga(false), isForcedByFlag(false),
        onGround(false), jump(false), runSprite(false), runTime(0.0f)
    {
    //
}

void Player::Update(float dt) {
    if (isForcedByFlag && !isRemoved) {
        // Sliding down the End Flag
        if (hitBox.y + hitBox.h < buttomOfFlag) {
            hitBox.y += 0.2f * dt;
            onGround = false;
            rigidBody.Reset();
            return;
        }
        // Moves compulsively
        else {
            hasCollided = false;
            rigidBody.Update(dt);

            rigidBody.ApplyForceX(0.0f);
            rigidBody.ApplyVelocityX(0.2f);
            
            hitBox.x += (int) rigidBody.GetPosition().x;
            hitBox.y += (int) rigidBody.GetPosition().y;

            position = hitBox.x + hitBox.w;
        }
        return;
    }

    onGround = false;
    hasCollided = false;
    rigidBody.Update(dt);
    hitBox.x += (int) rigidBody.GetPosition().x;
    
    if (hitBox.x - GameObject::screen.x < 5) {
        hitBox.x = GameObject::screen.x + 5;
        rigidBody.ApplyVelocityX(0.0f);
    }
        
    if (hitBox.x + hitBox.w >= GameObject::screen.x + GameObject::screen.w - 5) {
        hitBox.x = GameObject::screen.x + GameObject::screen.w - hitBox.w - 5;
        rigidBody.ApplyVelocityX(0.0f);
    }
        
    hitBox.y += (int) rigidBody.GetPosition().y;

    position = hitBox.x + hitBox.w;
}

void Player::Render() {
    #ifdef COLLISION
    rectangleRGBA(window->GetRenderer(), texture.DestRect().x, texture.DestRect().y, texture.DestRect().x + texture.DestRect().w, texture.DestRect().y + texture.DestRect().h, 255, 0, 0, 255);
    #endif

    if (isGiga) {
        texture.Render();
        return;
    }

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
}

void Player::Reset() {
    rigidBody.Reset();
    hitBox = spawnPoint;
    previousPosition = spawnPoint;
    texture.DestRect() = spawnPoint;
    isRemoved = false;
    isForcedByFlag = false;
    isGiga = false;
    playSound = false;
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

    rigidBody.Reset();
    rigidBody.ApplyVelocityY(-100.0f);
}

void Player::MakeGiga() {
    isGiga = true;
    playSound = true;
    if (faceLeft)
        texture.SrcRect() = {96, 0, 41, 68};
    else
        texture.SrcRect() = {137, 0, 41, 68};
    
    texture.DestRect().w = 103;
    texture.DestRect().h = 170;
}

void Player::SetButtomOfFlag(int y) {
    isForcedByFlag = true;
    buttomOfFlag = y;
}

bool Player::IsForcedByFlag() const { return isForcedByFlag; }

bool& Player::OnGround() { return onGround; }

SDL_Rect& Player::SpawnPoint() { return spawnPoint; }

int Player::GetPosition() { return position; }

Player::~Player() {
    #ifdef DTOR
    std::clog << "~Player Dtor" << std::endl;
    #endif
}
/* ************************************************************************************ */

/***** Class Enemy *****/
Enemy::Enemy(SDL_Rect hitBox, int activationPoint, SDL_Rect srcRect, int shiftTextureRight, SDL_Rect destRect, bool faceLeft)
    : Entity(hitBox, srcRect, destRect, faceLeft), faceLeftDefault(faceLeft), isActivated(false),
        activationPoint(activationPoint), shiftTextureRight(shiftTextureRight)
    {
    //
}

void Enemy::Update(float dt) {
    if ((activationPoint > Player::GetPosition() && !isActivated) || isRemoved)
        return;

    isActivated = true;
    hasCollided = false;
    rigidBody.Update(dt);
    
    hitBox.x += (int) rigidBody.GetPosition().x;
    hitBox.y += (int) rigidBody.GetPosition().y;

    if (faceLeft)
        rigidBody.ApplyVelocityX(-0.1f);
    else
        rigidBody.ApplyVelocityX(0.1f);
}

void Enemy::Render() {
    // Dest Rect:
    #ifdef COLLISION
    rectangleRGBA(window->GetRenderer(), texture.DestRect().x, texture.DestRect().y, texture.DestRect().x + texture.DestRect().w, texture.DestRect().y + texture.DestRect().h, 255, 0, 0, 255);
    #endif

    if (isActivated == false || isRemoved)
        return;

    SDL_Rect tempSrcRect = texture.SrcRect();
    if (faceLeft == false)
        tempSrcRect.x += shiftTextureRight;

    SDL_RenderCopy(Texture::renderer, texture.GetTexture(), &tempSrcRect, &texture.DestRect());
}

void Enemy::Reset() {
    rigidBody.Reset();
    hitBox = spawnPoint;
    previousPosition = spawnPoint;
    texture.DestRect() = spawnPoint;
    isRemoved = false;
    recoverX = false;
    recoverY = false;
    isActivated = false;
    playSound = false;
    faceLeft = faceLeftDefault;
}

void Enemy::Touch(GameObject* object) {
    object->TouchedBy(this);
}

void Enemy::Kill() { isRemoved = true; }

Enemy::~Enemy() {
    #ifdef DTOR
    std::clog << "~Enemy Dtor" << std::endl;
    #endif
}
/* ************************************************************************************ */

/***** Class Common Enemy *****/
CommonEnemy::CommonEnemy(SDL_Rect hitBox, int activationPoint, SDL_Rect srcRect, int shiftTextureRight, SDL_Rect destRect, bool faceLeft)
    : Enemy(hitBox, activationPoint, srcRect, shiftTextureRight, destRect, faceLeft) {
    //
}

void CommonEnemy::TouchedBy(Entity* entity) {
    if (dynamic_cast<Enemy*>(entity))
        return;
    else if (Player* player = dynamic_cast<Player*>(entity)) {
        if (GameObject::JumpedOnHead(player->HitBox(), this->HitBox()) && player->IsDead() == false) {
            this->Kill();
            playSound = true;
            player->GetRigidBody().ApplyVelocityY(-1.0f);
        }
        else
            player->Kill();
        return;
    }
    else
        throw "Unknown entity!";
}

CommonEnemy::~CommonEnemy() {
    #ifdef DTOR
    std::clog << "~CommonEnemy Dtor" << std::endl;
    #endif
}
/* ************************************************************************************ */

/***** Class Sodier Enemy *****/
SoldierEnemy::SoldierEnemy(SDL_Rect hitBox, int activationPoint, SDL_Rect srcRect, int shiftTextureRight, SDL_Rect destRect, bool faceLeft)
    : Enemy(hitBox, activationPoint, srcRect, shiftTextureRight, destRect, faceLeft), isSqueezed(false), isSliding(false) {
    //
}

void SoldierEnemy::Update(float dt) {
    Enemy::Update(dt);
    if (isActivated && !isRemoved && isSqueezed) {
        if (isSliding) {
            if (faceLeft)
                rigidBody.ApplyVelocityX(-0.5f);
            else
                rigidBody.ApplyVelocityX(0.5f);
        }
        else
            rigidBody.ApplyVelocityX(0.0f);
    }
}

void SoldierEnemy::Reset() {
    Enemy::Reset();
    isSqueezed = false;
    isSliding = false;
    texture.SrcRect() = {0, 95, 30, 38};
}

void SoldierEnemy::TouchedBy(Entity* entity) {
    if (dynamic_cast<Enemy*>(entity)) {
        if (this->isSliding == false)
            return;

        // Checking if both are sliding Soldiers
        if (SoldierEnemy* otherSoldier = dynamic_cast<SoldierEnemy*>(entity))
            if (otherSoldier->isSliding)
                this->Kill();
        
        entity->Kill();
    }
    else if (Player* player = dynamic_cast<Player*>(entity)) {
        if (player->IsDead())
            return;
        // Squeeze Down
        if (this->isSqueezed == false) {
            if (GameObject::JumpedOnHead(player->HitBox(), this->hitBox)) {
                this->isSqueezed = true;
                this->playSound = true;
                this->texture.SrcRect() = {0, 133, 30, 26};
                this->texture.DestRect().w = 75;
                this->texture.DestRect().h = 65;
                this->hitBox.w = 75;
                this->hitBox.h = 65;
                player->GetRigidBody().ApplyVelocityY(-10.0f);
            }
            else
                player->Kill();
        }
        // Start Sliding
        else if (this->isSqueezed && this->isSliding == false) {
            if (GameObject::JumpedOnHead(player->HitBox(), this->hitBox)) {
                this->isSliding = true;
                this->playSound = true;
                player->GetRigidBody().ApplyVelocityY(-10.0f);
            }
            else
                player->Kill();
        }
        // Stop Sliding
        else if (this->isSqueezed && this->isSliding) {
            if (GameObject::JumpedOnHead(player->HitBox(), this->hitBox)) {
                this->isSliding = false;
                this->playSound = true;
                player->GetRigidBody().ApplyVelocityY(-10.0f);   
            }
            else
                player->Kill();
        }
        
        return;
    }
    else
        throw "Unknown entity!";
}

bool SoldierEnemy::IsSqueezed() const { return isSqueezed; }

SoldierEnemy::~SoldierEnemy() {
    #ifdef DTOR
    std::clog << "~SoldierEnemy Dtor" << std::endl;
    #endif
}
/* ************************************************************************************ */

/***** Class King Enemy *****/
KingEnemy::KingEnemy(SDL_Rect hitBox, int activationPoint, SDL_Rect srcRect, int shiftTextureRight, SDL_Rect destRect, bool faceLeft)
    : Enemy(hitBox, activationPoint, srcRect, shiftTextureRight, destRect, faceLeft) {
    //
}

void KingEnemy::TouchedBy(Entity* entity) {
    if (dynamic_cast<Enemy*>(entity))
        return;
    else if (Player* player = dynamic_cast<Player*>(entity)) {
        player->Kill();
        return;
    }
    else
        throw "Unknown entity!";
}

KingEnemy::~KingEnemy() {
    #ifdef DTOR
    std::clog << "~KingEnemy Dtor" << std::endl;
    #endif
}
/* ************************************************************************************ */

/***** Class Red Mushroom Enemy *****/
RedMushroomEnemy::RedMushroomEnemy(SDL_Rect hitBox, int activationPoint, SDL_Rect srcRect, int shiftTextureRight, SDL_Rect destRect, bool faceLeft)
    : Enemy(hitBox, activationPoint, srcRect, shiftTextureRight, destRect, faceLeft) {
    //
}

void RedMushroomEnemy::TouchedBy(Entity* entity) {
    if (dynamic_cast<Enemy*>(entity))
        return;
    else if (Player* player = dynamic_cast<Player*>(entity)) {
        this->Kill();
        player->MakeGiga();
        return;
    }
    else
        throw "Unknown entity!";
}

RedMushroomEnemy::~RedMushroomEnemy() {
    #ifdef DTOR
    std::clog << "~RedMushroomEnemy Dtor" << std::endl;
    #endif
}
/* ************************************************************************************ */

/***** Class Purple Mushroom Enemy *****/
PurpleMushroomEnemy::PurpleMushroomEnemy(SDL_Rect hitBox, int activationPoint, SDL_Rect srcRect, int shiftTextureRight, SDL_Rect destRect, bool faceLeft)
    : Enemy(hitBox, activationPoint, srcRect, shiftTextureRight, destRect, faceLeft) {
    //
}

void PurpleMushroomEnemy::TouchedBy(Entity* entity) {
    if (dynamic_cast<Enemy*>(entity))
        return;
    else if (Player* player = dynamic_cast<Player*>(entity)) {
        player->Kill();
        return;
    }
    else
        throw "Unknown entity!";
}

PurpleMushroomEnemy::~PurpleMushroomEnemy() {
    #ifdef DTOR
    std::clog << "~PurpleMushroomEnemy Dtor" << std::endl;
    #endif
}
#endif // CPORTA