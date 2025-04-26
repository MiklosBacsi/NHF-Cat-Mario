#ifndef CPORTA

#include <SDL2/SDL.h>

#include <iostream>

#include "Entity.h"
#include "RigidBody.h"

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

Player::~Player() {
    #ifdef DTOR
    std::clog << "~Player Dtor" << std::endl;
    #endif
}
/* ************************************************************************************ */

/***** Class Enemy *****/
Enemy::Enemy() {}

void Enemy::update(float dt) {}

Enemy::~Enemy() {
    #ifdef DTOR
    std::clog << "~Enemy Dtor" << std::endl;
    #endif
}
#endif // CPORTA