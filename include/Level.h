#ifndef LEVEL_H
#define LEVEL_H

#ifndef CPORTA

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <memory>

#include "RenderWindow.h"
#include "GameObject.h"
#include "Texture.h"
#include "RigidBody.h"
#include "Entity.h"
#include "Block.h"
#include "LevelElement.h"
#include "CoinAnimation.h"

#define BLOCK_SIZE 30
#define SCALED_BLOCK_SIZE 75

// Forward declaration
class RenderWindow;

struct Coordinate {
    int x;
    int y;
};

struct EnemyData {
    Coordinate spawnPoint;
    int activationPoint;
    bool faceLeft;
};

class Level {
    friend class GameEngine;
public:
    enum Type { NONE=0, LVL1, LVL2 };
    static bool isCompleted;
private:
    std::unique_ptr<Player> player;
    std::vector<std::unique_ptr<Enemy>> enemies;
    std::vector<std::unique_ptr<Enemy>> tempEnemies;
    Grid grid;
    std::vector<std::unique_ptr<LevelElement>> elements;
    CoinAnimation animation;
    Quote quote;
    Enemy* enemyWithQuote;
    std::unique_ptr<TextButton> quoteButton;
    int maxCameraX;

    void AddPlayer(int x, int y);
    void AddUpperDirtBlock(int row, int column);
    void AddLowerDirtBlock(int row, int column);
    void AddBrickBlock(int row, int column);
    void AddBoxyBlock(int row, int column);
    void AddHiddenBlock(int row, int column);
    void AddMysteryBlock(int row, int column);
    void AddCommonEnemy(int x, int y, int activationPoint, bool faceLeft);
    void AddSoldierEnemy(int x, int y, int activationPoint, bool faceLeft);
    void AddKingEnemy(int x, int y, int activationPoint, bool faceLeft);
    void AddRedMushroomEnemy(int x, int y, int activationPoint, bool faceLeft);
    void AddPurpleMushroomEnemy(int x, int y, int activationPoint, bool faceLeft);
    void AddHill(int x, int y);
    void AddTree(int x, int y);
    void AddGrass(int x, int y);
    void AddCloud(int x, int y);
    void AddFish(int x, int y, int activationPoint, bool faceUp);
    void AddLaser(int x, int y, int activationPoint, bool faceLeft);
    void AddHighTube(int x, int y);
    void AddMiddleTube(int x, int y);
    void AddLowTube(int x, int y);
    void AddCheckpointFlag(int x, int y);
    void AddEndFlag(int x, int y);
    void AddHouse(int x, int y);

    void LoadLevelFromConfigFile(std::string configFile);
    bool ParseBool(const std::string& token);
    int ReadInt(std::ifstream& in);
    Coordinate ReadCoordinate(std::ifstream& in);
    EnemyData ReadEnemyData(std::ifstream& in);
public:
    Level(std::string configFile, RenderWindow* window, int frameDelay);
    void Update(float dt);
    void Render();
    void Reset();
    ~Level();
};

#endif // CPORTA

#endif // LEVEL_H