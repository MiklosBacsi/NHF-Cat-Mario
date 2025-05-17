/** @file Block.h
  * @brief Blocks in the game, which are arranged in a grid
  *  
  * This file contains the declarations class Block and its
  * descendants, as well as the Grid containing them using
  * heterogeneous collection.
  *
  * @author Bácsi Miklós
  * @date 2025-05-17
*/

#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#ifndef CPORTA

#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"

#include <iostream>

#include "Texture.h"
#include "RigidBody.h"
#include "RenderWindow.h"

// Forward declaration
class Entity;
class GameEngine;

/**
 * @brief Virtual base class that allows compatibility between game objects.
 */
class GameObject {
public:
    static RenderWindow* window;    ///< Static member, necessary for accessing information.
    static SDL_Rect screen;         ///< Static member, represents how to screen is moved.

    /**
     * @brief Checks whether the two rectangles intersect.
     * @param A Reference to the first hitbox.
     * @param B Reference to the second hitbox.
     * @see SDL_Rect
     * @return Returns whether the rectangles intersect.
     */
    static bool AABB(const SDL_Rect& A, const SDL_Rect& B);

    /**
     * @brief Checks whether the two rectangles intersect.
     * @param A Reference to the first hitbox. This is the reference rectangle.
     * @param B Reference to the second hitbox. This is the comparison rectangle.
     * @see SDL_Rect
     * @return Returns how much A overhangs B on the right, returns 0 if they don't.
     */
    static int OverhangRight(const SDL_Rect& A, const SDL_Rect& B);

    /**
     * @brief Checks whether the two rectangles intersect.
     * @param A Reference to the first hitbox. This is the reference rectangle.
     * @param B Reference to the second hitbox. This is the comparison rectangle.
     * @see SDL_Rect
     * @return Returns whether A overhangs B on the left, returns 0 if they don't.
     */
    static int OverhangLeft(const SDL_Rect& A, const SDL_Rect& B);

    /**
     * @brief Checks whether the two rectangles intersect.
     * @param A Reference to the first hitbox. This is the reference rectangle.
     * @param B Reference to the second hitbox. This is the comparison rectangle.
     * @see SDL_Rect
     * @return Returns whether A overhangs B on the top, returns 0 if they don't.
     */
    static int OverhangUp(const SDL_Rect& A, const SDL_Rect& B);

    /**
     * @brief Checks whether the two rectangles intersect.
     * @param A Reference to the first hitbox. This is the reference rectangle.
     * @param B Reference to the second hitbox. This is the comparison rectangle.
     * @see SDL_Rect
     * @return Returns whether A overhangs B on the button, returns 0 if they don't.
     */
    static int OverhangDown(const SDL_Rect& A, const SDL_Rect& B);

    /**
     * @brief Checks whether the Player jumped on top of the Enemy.
     * @param A Reference to the hitbox of the Player.
     * @param B Reference to the hitbox of the Enemy.
     * @see SDL_Rect
     * @return Returns whether the Player jumped on top of the Enemy.
     */
    static bool JumpedOnHead(const SDL_Rect& player, const SDL_Rect& enemy);

protected:
    Texture texture;    ///< Texture of the GameObject.
    SDL_Rect hitBox;    ///< Hitbox the GameObject.
    bool isRemoved;     ///< Represents an object beeing destroyed.

public:
    /**
     * Creates a new GameObject with the given boxes and texture.
     * @param hitBox Hitbox of the GameObject.
     * @param srcRect Source rectangle for the Texture.
     * @param destRect Destination rectangle for the Texture.
     * @param texture Pointer to the texture of the GameObject.
     * @param isRemoved Represents an object beeing destroyed.
     * @see SDL_Rect
     * @see SDL_Texture
     */
    GameObject(SDL_Rect hitBox, SDL_Rect srcRect, SDL_Rect destRect, SDL_Texture* texture, bool isRemoved=false);

    /**
     * @brief Updates the object with delta time.
     * @param dt Delta time
     */
    virtual void Update(float dt) = 0;

    /**
     * @brief Renders the object to the screen (renderer).
     */
    virtual void Render() = 0;

    /**
     * @brief Resets the object to default state.
     */
    virtual void Reset() = 0;

    /**
     * @brief Handles what happens when it is touched by an Entity.
     * @param entity Pointer to Entity that this object is touched by.
     */
    virtual void TouchedBy(Entity* entity) = 0;

    /**
     * @brief Returns the hitbox of the object as reference.
     * @return Reference to the hitbox of the object.
     */
    virtual SDL_Rect& HitBox();

    /**
     * @brief Updates the destination rectangles of the texture, so that they would fit correctly on the screen.
     */
    virtual void UpdateDestRect();

    /**
     * @brief Virtual destructor.
     */
    virtual ~GameObject();
};

#endif // CPORTA

#endif // GAME_OBJECT_H