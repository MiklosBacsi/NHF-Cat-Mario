#ifndef BLOCK_H
#define BLOCK_H

#ifndef CPORTA

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <iostream>

#include "GameObject.h"
#include "Texture.h"
#include "RigidBody.h"

// Forward declaration
class Entity;

class Block : public GameObject {
public:
    static SDL_Texture* textures;

    Block(SDL_Rect hitBox, SDL_Rect srcRect, SDL_Rect destRect);
    virtual void Update(float dt);
    virtual void Render();
    virtual void Reset();
    virtual void TouchedBy(Entity* entity);
    virtual void Limit(Entity* entity);
    ~Block();
};

class Grid {
private:
    int width;
    int height;
    int blockSize;
    std::vector<std::unique_ptr<Block>> blocks;
public:
    Grid(int width, int height, int blockSize);
    std::unique_ptr<Block>& operator()(int row, int column);
    std::unique_ptr<Block>& operator[](int index);
    int Size() const;
    void Update(float dt);
    void Render();
    void Reset();
    void UpdateDestRect();
    void CheckCollision(Entity* entity);
    ~Grid();
};

#endif // CPORTA

#endif // BLOCK_H