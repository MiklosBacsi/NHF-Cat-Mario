#ifndef CPORTA

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <memory>

#include "Level.h"
#include "GameObject.h"
#include "Texture.h"
#include "RigidBody.h"
#include "Entity.h"
#include "Block.h"
#include "LevelElement.h"
#include "RenderWindow.h"

bool Level::isCompleted = false;

Level::Level(std::string configFile, RenderWindow* window, int frameDelay)
    : player(nullptr), grid(SCALED_BLOCK_SIZE), animation("../res/img/Coin.png",
        (float) frameDelay / 1000.0f), enemyWithQuote(nullptr)
    {
    GameObject::window = window;
    GameObject::screen = {0, 0, window->GetWidth(), window->GetHeight()};
    isCompleted = false;

    Block::textures = Texture::LoadStaticTexture("../res/img/Block.png");
    Entity::textures = Texture::LoadStaticTexture("../res/img/Entity.png");
    LevelElement::textures = Texture::LoadStaticTexture("../res/img/LevelElement.png");

    LoadLevelFromConfigFile(configFile);
}

void Level::Update(float dt) {
    player->Update(dt);
    
    for (auto& enemy : enemies)
        enemy->Update(dt);
    for (auto& enemy : tempEnemies)
        enemy->Update(dt);

    grid.Update(dt);
    
    for (auto& element : elements)
        element->Update(dt);

    if (GameObject::screen.x + (GameObject::screen.w / 2) < player->HitBox().x + player->HitBox().w)
        GameObject::screen.x = player->HitBox().x + player->HitBox().w - (GameObject::screen.w / 2);
}

void Level::Render() {
    GameObject::window->DrawBackground();

    for (auto& element : elements)
        element->Render();

    animation.Render(GameObject::screen.x, GameObject::window->GetRenderer());

    grid.Render();

    for (auto& enemy : enemies)
        enemy->Render();
    for (auto& enemy: tempEnemies)
        enemy->Render();

    if (quoteButton != nullptr)
        quoteButton->DrawButton();

    player->Render();
}

void Level::Reset() {
    GameObject::screen.x = 0;
    player->Reset();
    
    for (auto& enemy : enemies)
        enemy->Reset();
    
    grid.Reset();
    
    for (auto& element : elements)
        element->Reset();

    enemyWithQuote = nullptr;
    quoteButton.reset();

    // Delete temporary Enemies that are spawned by Mystery Blocks
    tempEnemies.clear();

    if (player->HitBox().x > 500)
        GameObject::screen.x = player->HitBox().x - 300;
}

Level::~Level() {
    if (Entity::textures != nullptr) {
        SDL_DestroyTexture(Entity::textures);
        Entity::textures = nullptr;
    }
    #ifdef DTOR
    std::clog << "~Level Dtor" << std::endl;
    #endif
}
/* ************************************************************************************ */

/***** Private Functions *****/
void Level::AddPlayer(int x, int y) {
    SDL_Rect srcRect = {0, 0, 24, 34};
    SDL_Rect hitBox = {x, y, 60, 85};

    player = std::make_unique<Player>(hitBox, srcRect, hitBox);
}

void Level::AddUpperDirtBlock(int row, int column) {
    SDL_Rect hitBox = {column * SCALED_BLOCK_SIZE, row * SCALED_BLOCK_SIZE, SCALED_BLOCK_SIZE, SCALED_BLOCK_SIZE};
    SDL_Rect srcRect = {0, 0, BLOCK_SIZE, BLOCK_SIZE};
    
    grid(row, column) = std::make_unique<Block>(hitBox, srcRect, hitBox);
}

void Level::AddLowerDirtBlock(int row, int column) {
    SDL_Rect hitBox = {column * SCALED_BLOCK_SIZE, row * SCALED_BLOCK_SIZE, SCALED_BLOCK_SIZE, SCALED_BLOCK_SIZE};
    SDL_Rect srcRect = {30, 0, BLOCK_SIZE, BLOCK_SIZE};
    
    grid(row, column) = std::make_unique<Block>(hitBox, srcRect, hitBox);
}

void Level::AddBrickBlock(int row, int column) {
    SDL_Rect hitBox = {column * SCALED_BLOCK_SIZE, row * SCALED_BLOCK_SIZE, SCALED_BLOCK_SIZE, SCALED_BLOCK_SIZE};
    SDL_Rect srcRect = {60, 0, BLOCK_SIZE, BLOCK_SIZE};
    
    grid(row, column) = std::make_unique<BrickBlock>(hitBox, srcRect, hitBox);
}

void Level::AddBoxyBlock(int row, int column) {
    SDL_Rect hitBox = {column * SCALED_BLOCK_SIZE, row * SCALED_BLOCK_SIZE, SCALED_BLOCK_SIZE, SCALED_BLOCK_SIZE};
    SDL_Rect srcRect = {90, 0, BLOCK_SIZE, BLOCK_SIZE};
    
    grid(row, column) = std::make_unique<Block>(hitBox, srcRect, hitBox);
}

void Level::AddHiddenBlock(int row, int column) {
    SDL_Rect hitBox = {column * SCALED_BLOCK_SIZE, row * SCALED_BLOCK_SIZE, SCALED_BLOCK_SIZE, SCALED_BLOCK_SIZE};
    SDL_Rect srcRect = {120, 0, BLOCK_SIZE, BLOCK_SIZE};
    
    grid(row, column) = std::make_unique<HiddenBlock>(hitBox, srcRect, hitBox);
}

void Level::AddMysteryBlock(int row, int column) {
    SDL_Rect hitBox = {column * SCALED_BLOCK_SIZE, row * SCALED_BLOCK_SIZE, SCALED_BLOCK_SIZE, SCALED_BLOCK_SIZE};
    SDL_Rect srcRect = {150, 0, BLOCK_SIZE, BLOCK_SIZE};
    
    grid(row, column) = std::make_unique<MysteryBlock>(hitBox, srcRect, hitBox);
}

void Level::AddCommonEnemy(int x, int y, int activationPoint, bool faceLeft) {
    SDL_Rect hitBox = {x, y, 75, 68};
    SDL_Rect srcRect = {0, 68, 30, 27};
    enemies.push_back(std::make_unique<CommonEnemy>(hitBox, activationPoint, srcRect, 30, hitBox, faceLeft));
}

void Level::AddSoldierEnemy(int x, int y, int activationPoint, bool faceLeft) {
    SDL_Rect hitBox = {x, y, 75, 92};
    SDL_Rect srcRect = {0, 95, 30, 38};
    enemies.push_back(std::make_unique<SoldierEnemy>(hitBox, activationPoint, srcRect, 30, hitBox, faceLeft));
}

void Level::AddKingEnemy(int x, int y, int activationPoint, bool faceLeft) {
    SDL_Rect hitBox = {x, y, 80, 82};
    SDL_Rect srcRect = {60, 68, 32, 33};
    enemies.push_back(std::make_unique<KingEnemy>(hitBox, activationPoint, srcRect, 32, hitBox, faceLeft));
}

void Level::AddRedMushroomEnemy(int x, int y, int activationPoint, bool faceLeft) {
    SDL_Rect hitBox = {x, y, 73, 73};
    SDL_Rect srcRect = {62, 131, 29, 29};
    enemies.push_back(std::make_unique<RedMushroomEnemy>(hitBox, activationPoint, srcRect, 29, hitBox, faceLeft));
}

void Level::AddPurpleMushroomEnemy(int x, int y, int activationPoint, bool faceLeft) {
    SDL_Rect hitBox = {x, y, 73, 75};
    SDL_Rect srcRect = {63, 101, 29, 30};
    enemies.push_back(std::make_unique<PurpleMushroomEnemy>(hitBox, activationPoint, srcRect, 29, hitBox, faceLeft));
}

void Level::AddHill(int x, int y) {
    SDL_Rect destRect = {x, y, 360, 158};
    SDL_Rect srcRect = {0, 40, 144, 63};
    elements.push_back(std::make_unique<LevelElement>(destRect, srcRect, destRect));
}

void Level::AddTree(int x, int y) {
    SDL_Rect destRect = {x, y, 65, 125};
    SDL_Rect srcRect = {122, 103, 26, 50};
    elements.push_back(std::make_unique<LevelElement>(destRect, srcRect, destRect));
}

void Level::AddGrass(int x, int y) {
    SDL_Rect destRect = {x, y, 162, 60};
    SDL_Rect srcRect = {122, 167, 65, 24};
    elements.push_back(std::make_unique<LevelElement>(destRect, srcRect, destRect));
}

void Level::AddCloud(int x, int y) {
    SDL_Rect destRect = {x, y, 175, 100};
    SDL_Rect srcRect = {0, 0, 70, 40};
    elements.push_back(std::make_unique<Cloud>(destRect, srcRect, destRect, 70));
}

void Level::AddFish(int x, int y, int activationPoint, bool faceUp) {
    SDL_Rect destRect = {x, y, 73, 105};
    SDL_Rect srcRect = {148, 83, 29, 42};
    if (!faceUp)
        srcRect.y = 125;
    elements.push_back(std::make_unique<Fish>(destRect, srcRect, destRect, activationPoint, faceUp));
}

void Level::AddLaser(int x, int y, int activationPoint, bool faceLeft) {
    SDL_Rect destRect = {x, y, 302, 40};
    SDL_Rect srcRect = {0, 285, 121, 16};
    elements.push_back(std::make_unique<Laser>(destRect, srcRect, destRect, activationPoint, faceLeft));
}

void Level::AddHighTube(int x, int y) {
    SDL_Rect destRect = {x, y, 152, 290};
    SDL_Rect srcRect = {0, 103, 61, 116};
    elements.push_back(std::make_unique<Tube>(destRect, srcRect, destRect));
}

void Level::AddMiddleTube(int x, int y) {
    SDL_Rect destRect = {x, y, 152, 220};
    SDL_Rect srcRect = {61, 103, 61, 88};
    elements.push_back(std::make_unique<Tube>(destRect, srcRect, destRect));
}

void Level::AddLowTube(int x, int y) {
    SDL_Rect destRect = {x, y, 152, 147};
    SDL_Rect srcRect = {0, 219, 61, 59};
    elements.push_back(std::make_unique<Tube>(destRect, srcRect, destRect));
}

void Level::AddCheckpointFlag(int x, int y) {
    SDL_Rect destRect = {x, y, 80, 150};
    SDL_Rect srcRect = {144, 0, 32, 60};
    elements.push_back(std::make_unique<CheckpointFlag>(destRect, srcRect, destRect));
}

void Level::AddEndFlag(int x, int y) {
    SDL_Rect destRect = {x, y, 55, 752};
    SDL_Rect srcRect = {187, 0, 22, 301};
    elements.push_back(std::make_unique<EndFlag>(destRect, srcRect, destRect));
}

void Level::AddHouse(int x, int y) {
    SDL_Rect destRect = {x, y, 250, 218};
    SDL_Rect srcRect = {83, 191, 100, 87};
    elements.push_back(std::make_unique<House>(destRect, srcRect, destRect));
}
/* ************************************************************************************ */

/***** Level Config Functions *****/
void Level::LoadLevelFromConfigFile(std::string configFile) {
    std::ifstream in(configFile);
    if (!in.is_open()) {
        std::cerr << "Failed to open config file: " << configFile << std::endl;
        return;
    }

    std::string line;
    while (std::getline(in, line)) {
        if (line == "Grid") {
            Coordinate coordinate = ReadCoordinate(in);
            grid.InitGrid(coordinate.x, coordinate.y);
        }
        else if (line == "Player") {
            Coordinate coordinate = ReadCoordinate(in);
            AddPlayer(coordinate.x, coordinate.y);
        }
        else if (line == "UpperDirtBlock") {
            int count = ReadInt(in);
            for (int i = 0; i < count; ++i) {
                Coordinate coordinate = ReadCoordinate(in);
                AddUpperDirtBlock(coordinate.x, coordinate.y);
            }
        }
        else if (line == "LowerDirtBlock") {
            int count = ReadInt(in);
            for (int i = 0; i < count; ++i) {
                Coordinate coordinate = ReadCoordinate(in);
                AddLowerDirtBlock(coordinate.x, coordinate.y);
            }
        }
        else if (line == "BrickBlock") {
            int count = ReadInt(in);
            for (int i = 0; i < count; ++i) {
                Coordinate coordinate = ReadCoordinate(in);
                AddBrickBlock(coordinate.x, coordinate.y);
            }
        }
        else if (line == "BoxyBlock") {
            int count = ReadInt(in);
            for (int i = 0; i < count; ++i) {
                Coordinate coordinate = ReadCoordinate(in);
                AddBoxyBlock(coordinate.x, coordinate.y);
            }
        }
        else if (line == "HiddenBlock") {
            int count = ReadInt(in);
            for (int i = 0; i < count; ++i) {
                Coordinate coordinate = ReadCoordinate(in);
                AddHiddenBlock(coordinate.x, coordinate.y);
            }
        }
        else if (line == "MysteryBlock") {
            int count = ReadInt(in);
            for (int i = 0; i < count; ++i) {
                Coordinate coordinate = ReadCoordinate(in);
                AddMysteryBlock(coordinate.x, coordinate.y);
            }
        }
        else if (line == "CommonEnemy") {
            int count = ReadInt(in);
            for (int i = 0; i < count; ++i) {
                EnemyData data = ReadEnemyData(in);
                AddCommonEnemy(data.spawnPoint.x, data.spawnPoint.y, data.activationPoint, data.faceLeft);
            }
        }
        else if (line == "SoldierEnemy") {
            int count = ReadInt(in);
            for (int i = 0; i < count; ++i) {
                EnemyData data = ReadEnemyData(in);
                AddSoldierEnemy(data.spawnPoint.x, data.spawnPoint.y, data.activationPoint, data.faceLeft);
            }
        }
        else if (line == "KingEnemy") {
            int count = ReadInt(in);
            for (int i = 0; i < count; ++i) {
                EnemyData data = ReadEnemyData(in);
                AddKingEnemy(data.spawnPoint.x, data.spawnPoint.y, data.activationPoint, data.faceLeft);
            }
        }
        else if (line == "RedMushroomEnemy") {
            int count = ReadInt(in);
            for (int i = 0; i < count; ++i) {
                EnemyData data = ReadEnemyData(in);
                AddRedMushroomEnemy(data.spawnPoint.x, data.spawnPoint.y, data.activationPoint, data.faceLeft);
            }
        }
        else if (line == "PurpleMushroomEnemy") {
            int count = ReadInt(in);
            for (int i = 0; i < count; ++i) {
                EnemyData data = ReadEnemyData(in);
                AddPurpleMushroomEnemy(data.spawnPoint.x, data.spawnPoint.y, data.activationPoint, data.faceLeft);
            }
        }
        else if (line == "Hill") {
            int count = ReadInt(in);
            for (int i = 0; i < count; ++i) {
                Coordinate coordinate = ReadCoordinate(in);
                AddHill(coordinate.x, coordinate.y);
            }
        }
        else if (line == "Tree") {
            int count = ReadInt(in);
            for (int i = 0; i < count; ++i) {
                Coordinate coordinate = ReadCoordinate(in);
                AddTree(coordinate.x, coordinate.y);
            }
        }
        else if (line == "Grass") {
            int count = ReadInt(in);
            for (int i = 0; i < count; ++i) {
                Coordinate coordinate = ReadCoordinate(in);
                AddGrass(coordinate.x, coordinate.y);
            }
        }
        else if (line == "Cloud") {
            int count = ReadInt(in);
            for (int i = 0; i < count; ++i) {
                Coordinate coordinate = ReadCoordinate(in);
                AddCloud(coordinate.x, coordinate.y);
            }
        }
        else if (line == "Fish") {
            int count = ReadInt(in);
            for (int i = 0; i < count; ++i) {
                EnemyData data = ReadEnemyData(in);
                AddFish(data.spawnPoint.x, data.spawnPoint.y, data.activationPoint, data.faceLeft);
            }
        }
        else if (line == "Laser") {
            int count = ReadInt(in);
            for (int i = 0; i < count; ++i) {
                EnemyData data = ReadEnemyData(in);
                AddLaser(data.spawnPoint.x, data.spawnPoint.y, data.activationPoint, data.faceLeft);
            }
        }
        else if (line == "HighTube") {
            int count = ReadInt(in);
            for (int i = 0; i < count; ++i) {
                Coordinate coordinate = ReadCoordinate(in);
                AddHighTube(coordinate.x, coordinate.y);
            }
        }
        else if (line == "MiddleTube") {
            int count = ReadInt(in);
            for (int i = 0; i < count; ++i) {
                Coordinate coordinate = ReadCoordinate(in);
                AddMiddleTube(coordinate.x, coordinate.y);
            }
        }
        else if (line == "LowTube") {
            int count = ReadInt(in);
            for (int i = 0; i < count; ++i) {
                Coordinate coordinate = ReadCoordinate(in);
                AddLowTube(coordinate.x, coordinate.y);
            }
        }
        else if (line == "CheckpointFlag") {
            int count = ReadInt(in);
            for (int i = 0; i < count; ++i) {
                Coordinate coordinate = ReadCoordinate(in);
                AddCheckpointFlag(coordinate.x, coordinate.y);
            }
        }
        else if (line == "EndFlag") {
            int count = ReadInt(in);
            for (int i = 0; i < count; ++i) {
                Coordinate coordinate = ReadCoordinate(in);
                AddEndFlag(coordinate.x, coordinate.y);
            }
        }
        else if (line == "House") {
            int count = ReadInt(in);
            for (int i = 0; i < count; ++i) {
                Coordinate coordinate = ReadCoordinate(in);
                AddHouse(coordinate.x, coordinate.y);
            }
        }
        else
            std::clog << "Could not find: " << line << std::endl;
    }

    in.close();
}

bool Level::ParseBool(const std::string& token) {
    if (token == "true")
        return true;
    if (token == "false")
        return false;
    throw std::runtime_error("Invalid boolean value: '" + token + "'");
}

int Level::ReadInt(std::ifstream& in) {
    std::string line;
    while (std::getline(in, line) && line.empty());

    try {
        return std::stoi(line);
    } catch (...) {
        throw std::runtime_error("Invalid integer value: '" + line + "'");
    }
}

Coordinate Level::ReadCoordinate(std::ifstream& in) {
    std::string line;
    while (std::getline(in, line) && line.empty());

    size_t open = line.find('(');
    size_t comma = line.find(',');
    size_t close = line.find(')');

    if (open == std::string::npos || comma == std::string::npos || close == std::string::npos || open > comma || comma > close)
        throw std::runtime_error("Invalid coordinate format: '" + line + "'");

    try {
        int x = std::stoi(line.substr(open + 1, comma - open - 1));
        int y = std::stoi(line.substr(comma + 1, close - comma - 1));
        return { x, y };
    } catch (...) {
        throw std::runtime_error("Failed to parse integers from coordinate: '" + line + "'");
    }
}

EnemyData Level::ReadEnemyData(std::ifstream& in) {
    std::string line;
    while (std::getline(in, line) && line.empty());

    std::istringstream ss(line);
    std::string coordStr;
    ss >> coordStr;

    size_t open = coordStr.find('(');
    size_t comma = coordStr.find(',');
    size_t close = coordStr.find(')');

    if (open == std::string::npos || comma == std::string::npos || close == std::string::npos)
        throw std::runtime_error("Invalid coordinate in enemy data: '" + coordStr + "'");

    int x, y, activationPoint;
    std::string boolStr;

    try {
        x = std::stoi(coordStr.substr(open + 1, comma - open - 1));
        y = std::stoi(coordStr.substr(comma + 1, close - comma - 1));
        ss >> activationPoint >> boolStr;
    } catch (...) {
        throw std::runtime_error("Failed to parse enemy data: '" + line + "'");
    }

    if (ss.fail() || boolStr.empty())
        throw std::runtime_error("Incomplete enemy data: '" + line + "'");

    return { {x, y}, activationPoint, ParseBool(boolStr) };
}

#endif // CPORTA