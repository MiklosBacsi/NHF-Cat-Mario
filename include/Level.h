#ifndef LEVEL_H
#define LEVEL_H

#ifndef CPORTA

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <iostream>
#include <string>
#include <vector>
#include <memory>

#include "GameObject.h"
#include "Texture.h"
#include "RigidBody.h"
#include "Entity.h"
#include "Block.h"
#include "LevelElement.h"
#include "Animation.h" // ???

#define LVL_WIDTH 20
#define LVL_HEIGHT 12
#define BLOCK_SIZE 30
#define SCALED_BLOCK_SIZE 75

// Forward declaration
class RenderWindow;

class Level {
    friend class GameEngine;
public:
    enum Type { NONE=0, LVL1, LVL2 };
private:
    std::unique_ptr<Player> player;
    std::vector<std::unique_ptr<Enemy>> enemies;
    Grid grid;
    std::vector<std::unique_ptr<LevelElement>> elements;
    size_t numberOfCheckpoints;
    size_t currentCheckpoint;
    std::string nextLevel;
public:
    Level(std::string configFile, RenderWindow* window);
    void Update(float dt);
    void Render();
    void Reset();
    ~Level();
};










#endif // CPORTA

#endif // LEVEL_H