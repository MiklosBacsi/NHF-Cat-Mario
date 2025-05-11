#ifndef LEVEL_ELEMENT_H
#define LEVEL_ELEMENT_H

#ifndef CPORTA

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <iostream>

#include "GameObject.h"
#include "Texture.h"
#include "RigidBody.h"

//Forward declaration
class Entity;

class LevelElement : public GameObject {
    friend class GameEngine;
public:
    static SDL_Texture* textures;
protected:
    bool isActivated;
    bool playSound;
public:
    LevelElement(SDL_Rect hitBox, SDL_Rect srcRect, SDL_Rect destRect);
    virtual void Update(float dt);
    virtual void Render();
    virtual void Reset();
    virtual void TouchedBy(Entity* entity);
    virtual ~LevelElement();
};

class Cloud : public LevelElement {
private:
    int shiftTextureRight;
public:
    Cloud(SDL_Rect hitBox, SDL_Rect srcRect, SDL_Rect destRect, int shiftTextureRight);
    void Render() override;
    void TouchedBy(Entity* entity) override;
    ~Cloud();
};

class Fish : public LevelElement {
private:
    SDL_Rect spawnPoint;
    int activationPoint;
    bool faceUp;
public:
    Fish(SDL_Rect hitBox, SDL_Rect srcRect, SDL_Rect destRect, int activationPoint, bool faceUp);
    void Update(float dt) override;
    void Reset() override;
    void TouchedBy(Entity* entity) override;
    ~Fish();
};

class Laser : public LevelElement {
private:
    SDL_Rect spawnPoint;
    int activationPoint;
    bool faceLeft;
public:
    Laser(SDL_Rect hitBox, SDL_Rect srcRect, SDL_Rect destRect, int activationPoint, bool faceLeft);
    void Update(float dt) override;
    void Reset() override;
    void TouchedBy(Entity* entity) override;
    ~Laser();
};

class Tube : public LevelElement {
public:
    Tube(SDL_Rect hitBox, SDL_Rect srcRect, SDL_Rect destRect);
    void TouchedBy(Entity* entity) override;
    ~Tube();
};


#endif // CPORTA

#endif // LEVEL_ELEMENT_H