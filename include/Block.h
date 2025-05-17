/** @file Block.h
  * @brief Blocks in the game, which are arranged in a grid
  *  
  * This file contains the declarations class Block and its
  * descendants, as well as the Grid containing them using
  * heterogeneous collection.
  *
  * @author Bácsi Miklós
  * @date 2025-05-18
*/

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

/**
 * @brief Simple Block class.
 */
class Block : public GameObject {
public:
    static SDL_Texture* textures;   ///< Static member. Pointer to all the Block textures.

    /**
     * Creates a new Block with the given boxes.
     * @param hitBox Hitbox of the Block.
     * @param srcRect Source rectangle for the Texture.
     * @param destRect Destination rectangle for the Texture.
     * @param isRemoved Represents an object beeing destroyed.
     * @see SDL_Rect
     */
    Block(SDL_Rect hitBox, SDL_Rect srcRect, SDL_Rect destRect, bool removed=false);

    /**
     * @brief Updates the block with delta time.
     * @param dt Delta time.
     */
    virtual void Update(float dt);

    /**
     * @brief Renders the block to the screen (renderer).
     */
    virtual void Render();

    /**
     * @brief Resets the block to default state.
     */
    virtual void Reset();

    /**
     * @brief Handles what happens when it is touched by an Entity.
     * @param entity Pointer to Entity that this object is touched by.
     */
    virtual void TouchedBy(Entity* entity);

    /**
     * @brief Limits the Entity to resolve collision.
     * @param entity Pointer to Entity that this object is touched by and we want to limit.
     */
    virtual void Limit(Entity* entity);

    /**
     * @brief Virtual destructor.
     */
    ~Block();
};


/**
 * @brief Hidden Block class that inherits from class Block.
 */
class HiddenBlock : public Block {
    friend class GameEngine;    ///< Necessary for playing animation.
private:
    bool playAnimation; ///< This is how the block shows that it wants to play an animation.
public:
    /**
     * Creates a new HiddenBlock with the given boxes.
     * @param hitBox Hitbox of the block.
     * @param srcRect Source rectangle for the Texture.
     * @param destRect Destination rectangle for the Texture.
     * @see SDL_Rect
     */
    HiddenBlock(SDL_Rect hitBox, SDL_Rect srcRect, SDL_Rect destRect);

    /**
     * @brief Resets the block to default state.
     */
    void Reset() override;

    /**
     * @brief Handles what happens when it is touched by an Entity.
     * @param entity Pointer to Entity that this object is touched by.
     */
    void TouchedBy(Entity* entity) override;

    /**
     * @brief Destructor.
     */
    ~HiddenBlock();
};


/**
 * @brief Brick Block class that inherits from class Block.
 */
class BrickBlock : public Block {
    friend class GameEngine;    ///< Necessary for playing animation.
private:
    bool playAnimation; ///< This is how the block shows that it wants to play an animation.
public:
    /**
     * Creates a new BrickBlock with the given boxes.
     * @param hitBox Hitbox of the block.
     * @param srcRect Source rectangle for the Texture.
     * @param destRect Destination rectangle for the Texture.
     * @see SDL_Rect
     */
    BrickBlock(SDL_Rect hitBox, SDL_Rect srcRect, SDL_Rect destRect);

    /**
     * @brief Handles what happens when it is touched by an Entity.
     * @param entity Pointer to Entity that this object is touched by.
     */
    void TouchedBy(Entity* entity) override;

    /**
     * @brief Destructor.
     */
    ~BrickBlock();
};


/**
 * @brief Mystery Block class that inherits from class Block.
 */
class MysteryBlock : public Block {
    friend class GameEngine;    ///< Necessary for playing animation.
private:
    bool playAnimation; ///< This is how the block shows that it wants to play an animation.
public:
    /**
     * Creates a new MysteryBlock with the given boxes.
     * @param hitBox Hitbox of the block.
     * @param srcRect Source rectangle for the Texture.
     * @param destRect Destination rectangle for the Texture.
     * @see SDL_Rect
     */
    MysteryBlock(SDL_Rect hitBox, SDL_Rect srcRect, SDL_Rect destRect);

    /**
     * @brief Renders the block to the screen (renderer).
     */
    void Render() override;

    /**
     * @brief Resets the block to default state.
     */
    void Reset() override;

    /**
     * @brief Handles what happens when it is touched by an Entity.
     * @param entity Pointer to Entity that this object is touched by.
     */
    void TouchedBy(Entity* entity) override;

    /**
     * @brief Destructor.
     */
    ~MysteryBlock();
};


/**
 * @brief Contains the grid flattened in a 1D array and uses a heterogeneous collection.
 */
class Grid {
    friend class GameEngine;    ///< Necessary for setting up the level.
private:
    int width;      ///< Width of the grid counted in blocks.
    int height;     ///< Height of the grid counted in blocks.
    int blockSize;  ///< Size of the blocks counted in pixels.
    std::vector<std::unique_ptr<Block>> blocks; ///< Contains the grid flattened in a 1D array and uses a heterogeneous collection.

public:
    /**
     * Creates a new Grid with the given block size.
     * @param blockSize Size of the blocks counted in pixels.
     * @see Block(SDL_Rect hitBox, SDL_Rect srcRect, SDL_Rect destRect, bool removed)
     */
    Grid(int blockSize);

    /**
     * @brief Initialises the grid with (width * height) blocks (nullptr).
     * @param width Width of the grid counted in blocks.
     * @param height Height of the grid counted in blocks.
     */
    void InitGrid(int width, int height);

    /**
     * @brief Operator() overload. Returns a pointer reference to the indexed block.
     * @param row Row index of block.
     * @param column Column index of block.
     * @return Returns a pointer reference to the indexed block.
     */
    std::unique_ptr<Block>& operator()(int row, int column);

    /**
     * @brief Operator[] overload. Returns a pointer reference to the indexed block.
     * @param index Absolute index of block (rowIndex * width + columnIndex).
     * @return Returns a pointer reference to the indexed block.
     */
    std::unique_ptr<Block>& operator[](int index);

    /**
     * @brief Return the number of non-empty blocks the grid contains.
     * @return Returns the number of blocks.
     */
    int Size() const;

    /**
     * @brief Updates every non-empty block in the grid.
     * @param dt Delta time.
     */
    void Update(float dt);

    /**
     * @brief Renders every non-empty block in the grid to the screen (renderer).
     */
    void Render();

    /**
     * @brief Resets every non-empty block in the grid to the default state.
     */
    void Reset();

    /**
     * @brief Updates all the blocks' destination rectangles of the textures in the grid, so that they would fit correctly on the screen.
     */
    void UpdateDestRect();

    /**
     * @brief Check whether the entity collides with any of the non-empty blocks.
     * @param entity Checked for collision.
     */
    void CheckCollision(Entity* entity);

    /**
     * @brief Destructor.
     */
    ~Grid();
};

#endif // CPORTA

#endif // BLOCK_H