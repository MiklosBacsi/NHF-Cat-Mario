#ifndef CPORTA

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <iostream>

#include "Entity.h"
#include "RigidBody.h"
#include "Animation.h"
#include "Texture.h"

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

void Player::update(float dt) {
    rigidBody.update(dt);
    transform += rigidBody.getPosition();
}

void Player::render(float cameraX) {}

Player::~Player() {
    #ifdef DTOR
    std::clog << "~Player Dtor" << std::endl;
    #endif
}
/* ************************************************************************************ */

/***** Class Enemy *****/
Enemy::Enemy() {}

void Enemy::update(float dt) {}

void Enemy::render(float cameraX) {}

Enemy::~Enemy() {
    #ifdef DTOR
    std::clog << "~Enemy Dtor" << std::endl;
    #endif
}
#endif // CPORTA