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

Level::Level(std::string configFile, RenderWindow* window) : screen({0,0,window->GetWidth()-1,window->GetHeight()-1}),
    player(nullptr)
    {
    GameObject::window = window;

    Entity::textures = Texture::LoadStaticTexture("../res/img/Entity.png");

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
}

void Level::Update(float dt) {
    player->Update(dt);
    
    for (auto& enemy : enemies)
        enemy->Update(dt);
    
    for (auto& block : blocks)
        block->Update(dt);
    
    for (auto& element : elements)
        element->Update(dt);

    // Some other logic might be required!!! %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
}

void Level::Render() {
    GameObject::window->DrawBackground();

    for (auto& element : elements)
        element->Render();

    for (auto& block : blocks)
        block->Render();

    for (auto& enemy : enemies)
        enemy->Render();

    player->Render();
}

void Level::Reset() {
    player->Reset();
    
    for (auto& enemy : enemies)
        enemy->Reset();
    
    for (auto& block : blocks)
        block->Reset();
    
    for (auto& element : elements)
        element->Reset();
}

Level::~Level() {
    #ifdef DTOR
    std::clog << "~Level Dtor" << endl;
    #endif
}

#endif // CPORTA