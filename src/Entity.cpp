#ifndef CPORTA

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <iostream>

#include "Entity.h"
#include "RigidBody.h"
#include "Texture.h"
#include "Animation.h" // ???

SDL_Texture* Entity::textures = nullptr;

/* ************************************************************************************ */

/***** Class Entity *****/
Entity::Entity() {}

Entity::~Entity() {
    #ifdef DTOR
    std::clog << "~Entity Dtor" << std::endl;
    #endif
}

/* ************************************************************************************ */

/***** Class Player *****/
Player::Player() {}

void Player::Update(float dt) {
    rigidBody.Update(dt);
    position += rigidBody.GetPosition();
}

void Player::Render() {}

void Player::Reset() {}

void Player::Touch(GameObject* object) {}

void Player::TouchedBy(Entity* entity) {
    Touch(entity);
}

Player::~Player() {
    #ifdef DTOR
    std::clog << "~Player Dtor" << std::endl;
    #endif
}
/* ************************************************************************************ */

/***** Class Enemy *****/
Enemy::Enemy() {}

void Enemy::Update(float dt) {}

void Enemy::Render() {}

Enemy::~Enemy() {
    #ifdef DTOR
    std::clog << "~Enemy Dtor" << std::endl;
    #endif
}
#endif // CPORTA