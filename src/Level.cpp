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
#include "Animation.h" // ???
#include "RenderWindow.h"

Level::Level(std::string configFile, RenderWindow* window) : player(nullptr), grid(LVL_WIDTH, LVL_HEIGHT, SCALED_BLOCK_SIZE) {
    GameObject::window = window;

    Entity::textures = Texture::LoadStaticTexture("../res/img/Entity.png");
    Block::textures = Texture::LoadStaticTexture("../res/img/Block.png");

    float Scale = 2.5f;
    int Width = 24;
    int Height = 35;
    int x = 200;
    int y = 100;
    float scaledWidth = (float) Width * Scale;
    float scaledHeight = (float) Height * Scale;
    SDL_Rect hitBox = {x, y, (int)scaledWidth, (int)scaledHeight};
    SDL_Rect srcRect = {0, 0, Width - 1, Height - 1};
    SDL_Rect destRect = {0, 0, (int)scaledWidth - 1, (int)scaledHeight - 1};

    player = std::make_unique<Player>(hitBox, srcRect, destRect);

    srcRect = {0, 0, BLOCK_SIZE-1, BLOCK_SIZE-1};


    for (int i=0; i < LVL_WIDTH; ++i) {
        hitBox = {i * SCALED_BLOCK_SIZE, 10 * SCALED_BLOCK_SIZE, SCALED_BLOCK_SIZE, SCALED_BLOCK_SIZE};
        grid(10,i) = std::make_unique<Block>(hitBox, srcRect, hitBox);
    }

    for (int i=10; i < LVL_WIDTH; ++i) {
        hitBox = {i * SCALED_BLOCK_SIZE, 9 * SCALED_BLOCK_SIZE, SCALED_BLOCK_SIZE, SCALED_BLOCK_SIZE};
        grid(9,i) = std::make_unique<Block>(hitBox, srcRect, hitBox);
    }

    // hitBox = {0 * scaledBlockSize, 10 * scaledBlockSize, scaledBlockSize, scaledBlockSize};
    // blocks.push_back(std::make_unique<UpperDirtBlock>(hitBox, srcRect));

    
}

void Level::Update(float dt) {
    player->Update(dt);
    
    for (auto& enemy : enemies)
        enemy->Update(dt);

    grid.Update(dt);
    
    for (auto& element : elements)
        element->Update(dt);

    // Some other logic might be required!!! %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    if (GameObject::screen.x + (GameObject::screen.w / 2) < player->HitBox().x + player->HitBox().w)
        GameObject::screen.x = player->HitBox().x + player->HitBox().w - (GameObject::screen.w / 2);
}

void Level::Render() {
    GameObject::window->DrawBackground();

    for (auto& element : elements)
        element->Render();

    grid.Render();

    for (auto& enemy : enemies)
        enemy->Render();

    player->Render();

    lineRGBA(Texture::renderer, GameObject::screen.x, GameObject::screen.y, GameObject::screen.x, GameObject::screen.y + GameObject::screen.h, 255, 0, 0, 255);
}

void Level::Reset() {
    player->Reset();
    
    for (auto& enemy : enemies)
        enemy->Reset();
    
    grid.Reset();
    
    for (auto& element : elements)
        element->Reset();

    GameObject::screen.x = 0;
}

Level::~Level() {
    #ifdef DTOR
    std::clog << "~Level Dtor" << endl;
    #endif
}

#endif // CPORTA