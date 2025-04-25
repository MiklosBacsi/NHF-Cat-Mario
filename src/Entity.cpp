#ifndef CPORTA

#include <SDL2/SDL.h>

#include <iostream>

#include "Entity.h"
#include "RigidBody.h"

/* ************************************************************************************ */

/***** Class Entity *****/

/* ************************************************************************************ */

/***** Class Player *****/
void Player::update(float dt) {
    rigidBody.update(dt);
    transform += rigidBody.getPosition();
}
/* ************************************************************************************ */

/***** Class Enemy *****/

#endif // CPORTA