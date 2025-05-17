/** @file LevelElement.h
  * @brief Contains declarations of base class LevelElement and its derived classes.
  * @author Bácsi Miklós
  * @date 2025-05-18
*/

#ifndef LEVEL_ELEMENT_H
#define LEVEL_ELEMENT_H

#ifndef CPORTA

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <iostream>

#include "GameObject.h"
#include "Texture.h"
#include "RigidBody.h"

//Forward declaration
class Entity;

/**
 * @brief Base class for elements allowing compatibility and heterogeneous collection. It inherits from GameObject.
 */
class LevelElement : public GameObject {
    friend class GameEngine;
public:
    static SDL_Texture* textures;   ///< Static member: stores all the block textures.

protected:
    bool isActivated;   ///< Contains whether the element is activated.
    bool playSound;     ///< This is how it shows that it wants to play a sound.

public:
    /**
     * @brief Creates a new LevelElement with the given boxes.
     * @param hitBox Hitbox of the object.
     * @param srcRect Source rectangle for the Texture.
     * @param destRect Destination rectangle for the Texture.
     */
    LevelElement(SDL_Rect hitBox, SDL_Rect srcRect, SDL_Rect destRect);

    /**
     * @brief Updates the element with delta time.
     * @param dt Delta time.
     */
    virtual void Update(float dt);

    /**
     * @brief Renders the element to the screen (renderer).
     */
    virtual void Render();

    /**
     * @brief Resets the element to default state.
     */
    virtual void Reset();

    /**
     * @brief Handles being touched by an Entity.
     * @param entity The entity which it is touched by.
     * @see Entity
     */
    virtual void TouchedBy(Entity* entity);

    /**
     * @brief Destructor: destroys static texture.
     */
    virtual ~LevelElement();
};

/***************************************************************************************************/

/**
 * @brief Cloud element derived from LevelElement.
 */
class Cloud : public LevelElement {
private:
    int shiftTextureRight;  ///< Contains how much the texture is shifted to the right (necessary for rendering the changed texture).
public:
    /**
     * @brief Creates a new Cloud with the given parameters.
     * @param hitBox Hitbox of the object.
     * @param srcRect Source rectangle for the Texture.
     * @param destRect Destination rectangle for the Texture.
     * @param shiftTextureRight Contains how much the texture is shifted to the right (necessary for rendering the changed texture).
     */
    Cloud(SDL_Rect hitBox, SDL_Rect srcRect, SDL_Rect destRect, int shiftTextureRight);

    /**
     * @brief Renders the cloud to the screen (renderer).
     */
    void Render() override;

    /**
     * @brief Handles being touched by an Entity.
     * @param entity The entity which it is touched by.
     * @see Entity
     */
    void TouchedBy(Entity* entity) override;

    /**
     * @brief Destructor.
     */
    ~Cloud();
};

/***************************************************************************************************/

/**
 * @brief Fish element derived from LevelElement.
 */
class Fish : public LevelElement {
private:
    SDL_Rect spawnPoint;    ///< Spawn point of object.
    int activationPoint;    ///< When the player reaches this x coordinate, the enemy gets activated.
    bool faceUp;            ///< Contains whether the object faces up (or down).
public:
    /**
     * @brief Creates a new Fish with the given boxes.
     * @param hitBox Hitbox of the object.
     * @param srcRect Source rectangle for the Texture.
     * @param destRect Destination rectangle for the Texture.
     * @param activationPoint When the player reaches this x coordinate, the enemy gets activated.
     * @param faceUp Contains whether the object faces up (or down).
     */
    Fish(SDL_Rect hitBox, SDL_Rect srcRect, SDL_Rect destRect, int activationPoint, bool faceUp);

    /**
     * @brief Updates the Fish with delta time.
     * @param dt Delta time.
     */
    void Update(float dt) override;

    /**
     * @brief Resets the Fish to default state.
     */
    void Reset() override;

    /**
     * @brief Handles being touched by an Entity.
     * @param entity The entity which it is touched by.
     * @see Entity
     */
    void TouchedBy(Entity* entity) override;

    /**
     * @brief Destructor.
     */
    ~Fish();
};

/***************************************************************************************************/

/**
 * @brief Laser element derived from LevelElement.
 */
class Laser : public LevelElement {
private:
    SDL_Rect spawnPoint;    ///< Spawn point of object.
    int activationPoint;    ///< When the player reaches this x coordinate, the enemy gets activated.
    bool faceLeft;  ///< Contains whether the object faces left (or right).
public:
    /**
     * @brief Creates a new Laser with the given boxes.
     * @param hitBox Hitbox of the object.
     * @param srcRect Source rectangle for the Texture.
     * @param destRect Destination rectangle for the Texture.
     * @param activationPoint When the player reaches this x coordinate, the enemy gets activated.
     * @param faceLeft Contains whether the object faces left (or right).
     */
    Laser(SDL_Rect hitBox, SDL_Rect srcRect, SDL_Rect destRect, int activationPoint, bool faceLeft);

    /**
     * @brief Updates the Laser with delta time.
     * @param dt Delta time.
     */
    void Update(float dt) override;

    /**
     * @brief Resets the Laser to default state.
     */
    void Reset() override;

    /**
     * @brief Handles being touched by an Entity.
     * @param entity The entity which it is touched by.
     * @see Entity
     */
    void TouchedBy(Entity* entity) override;

    /**
     * @brief Destructor.
     */
    ~Laser();
};

/***************************************************************************************************/

/**
 * @brief Tube element derived from LevelElement.
 */
class Tube : public LevelElement {
public:

    /**
     * @brief Creates a new Tube with the given boxes.
     * @param hitBox Hitbox of the object.
     * @param srcRect Source rectangle for the Texture.
     * @param destRect Destination rectangle for the Texture.
     */
    Tube(SDL_Rect hitBox, SDL_Rect srcRect, SDL_Rect destRect);

    /**
     * @brief Handles being touched by an Entity.
     * @param entity The entity which it is touched by.
     * @see Entity
     */
    void TouchedBy(Entity* entity) override;

    /**
     * @brief Destructor.
     */
    ~Tube();
};

/***************************************************************************************************/

/**
 * @brief Checkpoint flag element derived from LevelElement.
 */
class CheckpointFlag : public LevelElement {
private:
    bool isReached; ///< Contains wether the player has reached it.
public:
    /**
     * @brief Creates a new CheckpointFlag with the given boxes.
     * @param hitBox Hitbox of the object.
     * @param srcRect Source rectangle for the Texture.
     * @param destRect Destination rectangle for the Texture.
     */
    CheckpointFlag(SDL_Rect hitBox, SDL_Rect srcRect, SDL_Rect destRect);

    /**
     * @brief Handles being touched by an Entity.
     * @param entity The entity which it is touched by.
     * @see Entity
     */
    void TouchedBy(Entity* entity);

    /**
     * @brief Destructor.
     */
    ~CheckpointFlag();
};

/***************************************************************************************************/

/**
 * @brief End flag element derived from LevelElement.
 */
class EndFlag : public LevelElement {
public:
    /**
     * @brief Creates a new EndFlag with the given boxes.
     * @param hitBox Hitbox of the object.
     * @param srcRect Source rectangle for the Texture.
     * @param destRect Destination rectangle for the Texture.
     */
    EndFlag(SDL_Rect hitBox, SDL_Rect srcRect, SDL_Rect destRect);

    /**
     * @brief Handles being touched by an Entity.
     * @param entity The entity which it is touched by.
     * @see Entity
     */
    void TouchedBy(Entity* entity) override;

    /**
     * @brief Destructor.
     */
    ~EndFlag();
};

/***************************************************************************************************/

/**
 * @brief House element derived from LevelElement.
 */
class House : public LevelElement {
private:
    bool isReached; ///< Contains wether the player has reached it.
public:
    /**
     * @brief Creates a new House with the given boxes.
     * @param hitBox Hitbox of the house.
     * @param srcRect Source rectangle for the Texture.
     * @param destRect Destination rectangle for the Texture.
     */
    House(SDL_Rect hitBox, SDL_Rect srcRect, SDL_Rect destRect);

    /**
     * @brief Handles being touched by an Entity.
     * @param entity The entity which it is touched by.
     * @see Entity
     */
    void TouchedBy(Entity* entity) override;

    /**
     * @brief Destructor.
     */
    ~House();
};

#endif // CPORTA

#endif // LEVEL_ELEMENT_H