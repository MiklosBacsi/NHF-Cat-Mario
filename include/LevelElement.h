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
protected:
    static SDL_Texture* textures;
public:
    LevelElement();
    virtual void Update(float dt) = 0;
    virtual void Render() = 0;
    virtual void Reset() = 0;
    virtual void TouchedBy(Entity* entity) = 0;
    virtual ~LevelElement();
};

// RigidBody rigidBody; For: TrickyMyteryBox, FallingBlocks, Fish, Laser, EndFlag???
// Vector2D spawnPoint; For: TrickyMyteryBox, FallingBlocks, Fish, Laser, EndFlag???










#endif // CPORTA

#endif // LEVEL_ELEMENT_H