#ifndef CPORTA

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <iostream>
#include <string>
#include <memory>

#include "Level.h"
#include "GameObject.h"
#include "Texture.h"
#include "RigidBody.h"
#include "Entity.h"
#include "Block.h"
#include "LevelElement.h"
#include "RenderWindow.h"

Level::Level(std::string configFile, RenderWindow* window, int frameDelay) : player(nullptr),
    grid(LVL_WIDTH, LVL_HEIGHT, SCALED_BLOCK_SIZE), animation("../res/img/Coin.png",
    (float) frameDelay / 1000.0f), enemyWithQuote(nullptr)
    {
    GameObject::window = window;

    Entity::textures = Texture::LoadStaticTexture("../res/img/Entity.png");
    Block::textures = Texture::LoadStaticTexture("../res/img/Block.png");

    AddPlayer(200, 550);

    AddSoldierEnemy(1200, 100, 1000, true);
    AddCommonEnemy(1200, 100, 1000, false);
    AddKingEnemy(1150, 100, 1000, true);
    AddSoldierEnemy(1150, 100, 1000, false);
    AddPurpleMushroomEnemy(1100, 100, 1000, true);
    AddRedMushroomEnemy(1100, 100, 1000, false);

    // Upper Dirt
    for (int i=10; i < LVL_WIDTH; ++i)
        AddUpperDirtBlock(9, i);

    // Lower Dirt
    for (int i=0; i < LVL_WIDTH; ++i)
        AddLowerDirtBlock(10, i);

    // Boxy Block
    for (int i=6; i < 10; ++i)
        AddBoxyBlock(i, 5);
    for (int i=5; i < 10; ++i)
        AddBoxyBlock(i, 7);
    AddBoxyBlock(8, 10);
    AddBoxyBlock(8, 20);

    // Hidden Block
    // for (int i=0; i < 3; ++i)
    //     AddHiddenBlock(3+i, 13+i);

    // Brick Block
    // for (int i=0; i < 3; ++i)
    //     AddBrickBlock(3+i, 10+i);

    // Mytery Block
    // for (int i=0; i < 3; ++i)
    //     AddMysteryBlock(3+i, 21+i);
}

void Level::Update(float dt) {
    player->Update(dt);
    
    for (auto& enemy : enemies)
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
}

Level::~Level() {
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

#endif // CPORTA