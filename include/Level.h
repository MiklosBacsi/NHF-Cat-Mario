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

// Forward declaration
class RenderWindow;

class Level {
public:
    enum Type { NONE=0, LVL1, LVL2 };
private:
    std::unique_ptr<Player> player;
    std::vector<std::unique_ptr<Enemy>> enemies;
    std::vector<std::unique_ptr<Block>> blocks;
    std::vector<std::unique_ptr<LevelElement>> levelElements;
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