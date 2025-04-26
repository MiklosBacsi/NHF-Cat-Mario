#ifndef LEVEL_H
#define LEVEL_H

#ifndef CPORTA

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <iostream>
#include <string>
#include <vector>

#include "Entity.h"
#include "RigidBody.h"
#include "Animation.h"
#include "Texture.h"
#include "Block.h"
#include "Element.h"


class Level {
public:
    enum Type { LVL1, LVL2 };
private:
    float cameraX;
    Player player;
    std::vector<Enemy*> enemies;
    std::vector<Block*> blocks;
    std::vector<Element*> elements;
    size_t numberOfCheckpoints;
    size_t currentCheckpoint;
    std::string nextLevel;
public:
    Level(const char* path);
    void update(float dt);
    void render(float cameraX);
    ~Level();
};










#endif // CPORTA

#endif // LEVEL_H