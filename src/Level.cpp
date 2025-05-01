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

Level::Level(std::string configFile, RenderWindow* window) : player(nullptr) {
    GameObject::window = window;

    player = std::make_unique<Player>();
}

void Level::Update(float dt) {
    player->Update(dt);
    
    for (auto& enemy : enemies)
        enemy->Update(dt);
    
    for (auto& block : blocks)
        block->Update(dt);
    
    for (auto& element : levelElements)
        element->Update(dt);

    // Some other logic might be required!!! %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
}

void Level::Render() {
    GameObject::window->DrawBackground();

    for (auto& element : levelElements)
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
    
    for (auto& element : levelElements)
        element->Reset();
}

Level::~Level() {
    #ifdef DTOR
    std::clog << "~Level Dtor" << endl;
    #endif
}

#endif // CPORTA