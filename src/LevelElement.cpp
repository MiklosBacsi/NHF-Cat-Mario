#ifndef CPORTA

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <iostream>

#include "LevelElement.h"
#include "Texture.h"
#include "RigidBody.h"
#include "Entity.h"

SDL_Texture* LevelElement::textures = nullptr;

/* ************************************************************************************ */

/***** Class Level Element *****/
LevelElement::LevelElement(SDL_Rect hitBox, SDL_Rect srcRect, SDL_Rect destRect)
    : GameObject(hitBox, srcRect, destRect, LevelElement::textures), isActivated(false), playSound(false)
    {
    //
}

void LevelElement::Update(float dt) {
    isActivated = true;
}

void LevelElement::Render() {
    // Dest Rect:
    #ifdef COLLISION
    rectangleRGBA(window->GetRenderer(), texture.DestRect().x, texture.DestRect().y, texture.DestRect().x + texture.DestRect().w, texture.DestRect().y + texture.DestRect().h, 0, 255, 0, 255);
    #endif
    
    if (isRemoved || isActivated == false)
        return;
    
    texture.Render();
}

void LevelElement::Reset() {
    isRemoved = false;
    isActivated = false;
    playSound = false;
}

void LevelElement::TouchedBy(Entity* entity) {}

LevelElement::~LevelElement() {
    if (LevelElement::textures != nullptr) {
        SDL_DestroyTexture(LevelElement::textures);
        LevelElement::textures = nullptr;
    }
    #ifdef DTOR
    std::clog << "~LevelElement Dtor" << std::endl;
    #endif
}
/* ************************************************************************************ */

/***** Class Cloud *****/
Cloud::Cloud(SDL_Rect hitBox, SDL_Rect srcRect, SDL_Rect destRect, int shiftTextureRight)
    : LevelElement(hitBox, srcRect, destRect), shiftTextureRight(shiftTextureRight)
    {
    //
}

void Cloud::Render() {
    // Dest Rect:
    #ifdef COLLISION
    rectangleRGBA(window->GetRenderer(), texture.DestRect().x, texture.DestRect().y, texture.DestRect().x + texture.DestRect().w, texture.DestRect().y + texture.DestRect().h, 0, 255, 0, 255);
    #endif

    SDL_Rect tempSrcRect = texture.SrcRect();
    if (isRemoved)  // Evil
        tempSrcRect.x += shiftTextureRight;
    
    SDL_RenderCopy(Texture::renderer, texture.GetTexture(), &tempSrcRect, &texture.DestRect());
}

void Cloud::TouchedBy(Entity* entity) {
    if (dynamic_cast<Enemy*>(entity))
        return;
    else if (Player* player = dynamic_cast<Player*>(entity)) {
        player->Kill();
        this->isRemoved = true;    // Change to Evil
        return;
    }
    else
        throw "Unknown entity!";
}

Cloud::~Cloud() {
    #ifdef DTOR
    std::clog << "~Cloud Dtor" << std::endl;
    #endif
}
/* ************************************************************************************ */

/***** Class Fish *****/
Fish::Fish(SDL_Rect hitBox, SDL_Rect srcRect, SDL_Rect destRect, int activationPoint, bool faceUp)
    : LevelElement(hitBox, srcRect, destRect), spawnPoint(hitBox), activationPoint(activationPoint), faceUp(faceUp)
    {
    //
}

void Fish::Update(float dt) {
    if (isRemoved)
        return;
    if (!isActivated) {
        if (activationPoint < Player::GetPosition()) {
            isActivated = true;
            playSound = true;
        }
        else
            return;
    }

    if (faceUp)
        hitBox.y -= (int) (1.0f * dt);
    else
        hitBox.y += (int) (1.0f * dt);
}

void Fish::Reset() {
    LevelElement::Reset();
    hitBox = spawnPoint;
}

void Fish::TouchedBy(Entity* entity) {
    if (dynamic_cast<Enemy*>(entity))
        return;
    else if (Player* player = dynamic_cast<Player*>(entity)) {
        player->Kill();
        return;
    }
    else
        throw "Unknown entity!";
}

Fish::~Fish() {
    #ifdef DTOR
    std::clog << "~Fish Dtor" << std::endl;
    #endif
}
/* ************************************************************************************ */

/***** Class Laser *****/
Laser::Laser(SDL_Rect hitBox, SDL_Rect srcRect, SDL_Rect destRect, int activationPoint, bool faceLeft)
    : LevelElement(hitBox, srcRect, destRect), spawnPoint(hitBox), activationPoint(activationPoint), faceLeft(faceLeft)
    {
    //
}

void Laser::Update(float dt) {
    if (isRemoved)
        return;
    if (!isActivated) {
        if (activationPoint < Player::GetPosition()) {
            isActivated = true;
            playSound = true;
        }
        else
            return;
    }

    if (faceLeft)
        hitBox.x -= (int) (1.0f * dt);
    else
        hitBox.x += (int) (1.0f * dt);
}

void Laser::Reset() {
    LevelElement::Reset();
    hitBox = spawnPoint;
}

void Laser::TouchedBy(Entity* entity) {
    if (dynamic_cast<Enemy*>(entity))
        return;
    else if (Player* player = dynamic_cast<Player*>(entity)) {
        player->Kill();
        return;
    }
    else
        throw "Unknown entity!";
}

Laser::~Laser() {
    #ifdef DTOR
    std::clog << "~Laser Dtor" << std::endl;
    #endif
}
/* ************************************************************************************ */

/***** Class Tube *****/
Tube::Tube(SDL_Rect hitBox, SDL_Rect srcRect, SDL_Rect destRect)
    : LevelElement(hitBox, srcRect, destRect)
    {
    //
}

void Tube::TouchedBy(Entity* entity) {
    entity->LimitedBy(this);
}

Tube::~Tube() {
    #ifdef DTOR
    std::clog << "~Tube Dtor" << std::endl;
    #endif
}

#endif // CPORTA