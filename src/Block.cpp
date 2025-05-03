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
Block::Block(SDL_Rect hitBox, SDL_Rect srcRect, SDL_Rect destRect, bool removed)
    : GameObject(hitBox, srcRect, destRect, Block::textures, removed) {
    //
}

void Block::Update(float dt) {}

void Block::Render() {
    if (!isRemoved)
        texture.Render();
    // HitBox:
    // rectangleRGBA(window->GetRenderer(), hitBox.x, hitBox.y, hitBox.x + hitBox.w, hitBox.y + hitBox.h, 255, 0, 0, 255);
    // Dest Rect:
    #ifdef COLLISION
    rectangleRGBA(window->GetRenderer(), texture.DestRect().x, texture.DestRect().y, texture.DestRect().x + texture.DestRect().w, texture.DestRect().y + texture.DestRect().h, 0, 0, 255, 255);
    #endif
}

void Block::Reset() {
    texture.DestRect().x = hitBox.x;
    texture.DestRect().y = hitBox.y;
    isRemoved = false;
}

void Block::TouchedBy(Entity* entity) {
    Limit(entity);
}

void Block::Limit(Entity* entity) {
    entity->LimitedBy(this);
}

Block::~Block() {
    if (Block::textures != nullptr) {
        SDL_DestroyTexture(Block::textures);
        Block::textures = nullptr;
    }
    #ifdef DTOR
    std::clog << "~Block Dtor" << std::endl;
    #endif
}
/* ************************************************************************************ */

/***** Class Hidden Block *****/
HiddenBlock::HiddenBlock(SDL_Rect hitBox, SDL_Rect srcRect, SDL_Rect destRect) : Block(hitBox, srcRect,
    destRect, true), playAnimation(false) {
}

void HiddenBlock::Reset() {
    texture.DestRect().x = hitBox.x;
    texture.DestRect().y = hitBox.y;
    isRemoved = true;
}

void HiddenBlock::TouchedBy(Entity* entity) {
    if (isRemoved && GameObject::OverhangUp(entity->HitBox(), hitBox) && entity->GetRigidBody().Velocity().y < 0) {
        isRemoved = false;
        Limit(entity);
        playAnimation = true;
    }
    else if (!isRemoved)
        Limit(entity);
}

HiddenBlock::~HiddenBlock() {
    #ifdef DTOR
    std::clog << "~HiddenBlock Dtor" << std::endl;
    #endif
}
/* ************************************************************************************ */

/***** Class Brick Block *****/
BrickBlock::BrickBlock(SDL_Rect hitBox, SDL_Rect srcRect, SDL_Rect destRect) : Block(hitBox, srcRect,
    destRect, false), playAnimation(false) {
}

void BrickBlock::TouchedBy(Entity* entity) {
    if (!isRemoved && GameObject::OverhangUp(entity->HitBox(), hitBox) && entity->GetRigidBody().Velocity().y < 0) {
        isRemoved = true;
        Limit(entity);
        playAnimation = true;
    }
    else if (!isRemoved)
        Limit(entity);
}

BrickBlock::~BrickBlock() {
    #ifdef DTOR
    std::clog << "~BrickBlock Dtor" << std::endl;
    #endif
}
/* ************************************************************************************ */

/***** Class Mystery Block *****/
MysteryBlock::MysteryBlock(SDL_Rect hitBox, SDL_Rect srcRect, SDL_Rect destRect) : Block(hitBox, srcRect,
    destRect, false), playAnimation(false) {
}

void MysteryBlock::Render() {
    texture.Render();
    // Dest Rect:
    #ifdef COLLISION
    rectangleRGBA(window->GetRenderer(), texture.DestRect().x, texture.DestRect().y, texture.DestRect().x + texture.DestRect().w, texture.DestRect().y + texture.DestRect().h, 0, 0, 255, 255);
    #endif
}

void MysteryBlock::Reset() {
    texture.DestRect().x = hitBox.x;
    texture.DestRect().y = hitBox.y;
    isRemoved = false;
    texture.SrcRect().x = 150;
}

void MysteryBlock::TouchedBy(Entity* entity) {
    if (!isRemoved && GameObject::OverhangUp(entity->HitBox(), hitBox) && entity->GetRigidBody().Velocity().y < 0) {
        isRemoved = true;
        Limit(entity);
        playAnimation = true;
        texture.SrcRect().x = 120;
    }
    else
        Limit(entity);
}

MysteryBlock::~MysteryBlock() {
    #ifdef DTOR
    std::clog << "~MysteryBlock Dtor" << std::endl;
    #endif
}
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