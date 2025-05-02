#ifndef CPORTA

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <iostream>
#include <vector>
#include <memory>

#include "Block.h"
#include "Texture.h"
#include "RigidBody.h"
#include "Entity.h"

SDL_Texture* Block::textures = nullptr;

/* ************************************************************************************ */

/***** Class Block *****/
Block::Block(SDL_Rect hitBox, SDL_Rect srcRect, SDL_Rect destRect) : GameObject(hitBox, srcRect, destRect, Block::textures) {
    //
}

void Block::Update(float dt) {}

void Block::Render() {
    texture.Render();
    // HitBox:
    // rectangleRGBA(window->GetRenderer(), hitBox.x, hitBox.y, hitBox.x + hitBox.w, hitBox.y + hitBox.h, 255, 0, 0, 255);
    // Dest Rect:
    //rectangleRGBA(window->GetRenderer(), texture.DestRect().x, texture.DestRect().y, texture.DestRect().x + texture.DestRect().w, texture.DestRect().y + texture.DestRect().h, 0, 0, 255, 255);
}

void Block::Reset() {}

void Block::TouchedBy(Entity* entity) {
    //
    Limit(entity);
}

void Block::Limit(Entity* entity) {
    entity->LimitedBy(this);
}

Block::~Block() {
    SDL_DestroyTexture(Block::textures);
    #ifdef DTOR
    std::clog << "~Block Dtor" << std::endl;
    #endif
}
/* ************************************************************************************ */

/***** Class UpperDirtBlock *****/

/* ************************************************************************************ */

/***** CLASS RENDER_WINDOW *****/

/* ************************************************************************************ */

/***** Class Grid *****/
Grid::Grid(int width, int height, int blockSize) : width(width), height(height),
    blockSize(blockSize), blocks(width * height) {
}

std::unique_ptr<Block>& Grid::operator()(int row, int column) {
    return blocks.at(row * width + column);
}

std::unique_ptr<Block>& Grid::operator[](int index) {
    return blocks.at(index);
}

int Grid::Size() const { return blocks.size(); }

void Grid::Update(float dt) {
    int startColumn = GameObject::screen.x / blockSize;
    int endColumn = (GameObject::screen.x + GameObject::screen.w) / blockSize + 1;
    if (startColumn < 0) startColumn = 0;
    if (endColumn > width) endColumn = width;

    for (int row=0; row < height; ++row)
        for (int column=startColumn; column < endColumn; ++column)
            if (blocks.at(row * width + column) != nullptr)
                blocks.at(row * width + column)->Update(dt);
}

void Grid::Render() {
    int startColumn = GameObject::screen.x / blockSize;
    int endColumn = (GameObject::screen.x + GameObject::screen.w) / blockSize + 1;
    if (startColumn < 0) startColumn = 0;
    if (endColumn > width) endColumn = width;

    for (int row=0; row < height; ++row)
        for (int column=startColumn; column < endColumn; ++column)
            if (blocks.at(row * width + column) != nullptr)
                blocks.at(row * width + column)->Render();
}

void Grid::Reset() {
    for (auto& block : blocks)
        if (block != nullptr)
            block->Reset();
}

void Grid::UpdateDestRect() {
    int startColumn = GameObject::screen.x / blockSize - 1;
    int endColumn = (GameObject::screen.x + GameObject::screen.w) / blockSize + 2;
    if (startColumn < 0) startColumn = 0;
    if (endColumn > width) endColumn = width;

    for (int row=0; row < height; ++row)
        for (int column=startColumn; column < endColumn; ++column)
            if (blocks.at(row * width + column) != nullptr)
                blocks.at(row * width + column)->UpdateDestRect();
}

void Grid::CheckCollision(Entity* entity) {
    int startColumn = GameObject::screen.x / blockSize;
    int endColumn = (GameObject::screen.x + GameObject::screen.w) / blockSize + 1;
    if (startColumn < 0) startColumn = 0;
    if (endColumn > width) endColumn = width;

    for (int row=0; row < height; ++row)
        for (int column=startColumn; column < endColumn; ++column)
            if (blocks.at(row * width + column) != nullptr)
                if (GameObject::AABB(entity->HitBox(), blocks.at(row * width + column)->HitBox()))
                    blocks.at(row * width + column)->TouchedBy(entity);
}

Grid::~Grid() {
    #ifdef DTOR
    std::clog << "~Grid Dtor" << std::endl;
    #endif
}

#endif // CPORTA