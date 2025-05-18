/** @file Entity.h
  * @brief Contains declaration of base class Entity and its derived classes. 
  * @author Bácsi Miklós
  * @date 2025-05-18
*/

#ifndef ENTITY_H
#define ENTITY_H

#ifndef CPORTA

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <iostream>

#include "GameObject.h"
#include "Texture.h"
#include "RigidBody.h"
#include "Timer.h"

/**********************************************************************************************************************************************/

/**
 * @brief Abstract base class for entities allowing compatibility and heterogeneous collection. It inherits from GameObject.
 */
class Entity : public GameObject {
    friend class GameEngine;
public:
    static SDL_Texture* textures;   ///< Static member. Stores all the Entity textures.
protected:
    RigidBody rigidBody;    ///< Rigid body component.
    SDL_Rect spawnPoint;    ///< Spawn point that the entity is reset to.
    SDL_Rect previousPosition;  ///< Necessary for recovering from collision.
    bool hasCollided;   ///< Contains whether the entity has collided in a single update session.
    bool recoverX;  ///< x coordinate to recover to.
    bool recoverY;  ///< y coordinate to recover to.
    bool faceLeft;  ///< Contains whether entity faces to the left (false if it faces to the right).
    bool playSound; ///< This is how it shows that it wants to play a sound.

public:
    /**
     * Creates a new Entity with the given boxes and members.
     * @param hitBox Hitbox of the Entity.
     * @param srcRect Source rectangle for the Texture.
     * @param destRect Destination rectangle for the Texture.
     * @param faceLeft Contains whether the entity faces left (or right).
     * @see SDL_Rect
     */
    Entity(SDL_Rect hitBox, SDL_Rect srcRect, SDL_Rect destRect, bool faceLeft=true);

    /**
     * @brief Updates the entity with delta time.
     * @param dt Delta time.
     */
    virtual void Update(float dt) = 0;

    /**
     * @brief Renders the entity to the screen (renderer).
     */
    virtual void Render() = 0;

    /**
     * @brief Resets the entity to default state.
     */
    virtual void Reset() = 0;

    /**
     * @brief Touches another object.
     * @param object The touched object.
     * @see GameObject
     */
    virtual void Touch(GameObject* object) = 0;

    /**
     * @brief Handles being touched by another Entity.
     * @param entity The entity which it is touched by.
     * @see Entity
     */
    virtual void TouchedBy(Entity* entity) = 0;

    /**
     * @brief Resolves collision with other object.
     * @param object The object that it is limited by.
     * @see GameObject
     */
    virtual void LimitedBy(GameObject* object);

    /**
     * @return Returns whether entity is dead.
     */
    bool IsDead() const;

    /**
     * @return Returns whether entity has collided in this update session.
     */
    bool HasCollided() const;

    /**
     * @return Returns reference to the rigid body.
     */
    RigidBody& GetRigidBody();

    /**
     * @brief Updates previous position (necessary for handling collision later).
     */
    virtual void UpdatePreviousPosition();

    /**
     * @brief Kills the entity.
     */
    virtual void Kill() = 0;

    /**
     * @brief Destructor.
     */
    virtual ~Entity();
};

/**********************************************************************************************************************************************/

/**
 * @brief Player of the game, that inherits from class Entity.
 */
class Player : public Entity {
    friend class GameEngine;
public:
    static int position;    ///< x Static member. Contains x coordinate and width of its hit box (necessary for activating e.g. Fish or Laser).
    static int GetPosition();   ///< Necessary for comparison with Enemy's activationPoint.

private:
    int deathCount;     ///< Contains player's death count that is shown at every death scene.
    bool isGiga;        ///< Contains whether the player has changed to a giga cat.
    bool isForcedByFlag;    ///< Contains whether the player is forced by flag.
    int bottomOfFlag;   ///< y coordinate that is the bottom of the end flag.
    bool onGround;      ///< Contains whether the player in on the ground in a single update session.
    bool jump;  ///< Contains whether the player is jumping.
    Timer jumpTime; ///< Stores the current jump time of the player.
    bool runSprite; ///< Contains whether the player is on run sprite (or stand sprite).
    float runTime;  ///< Contains for how much time the player has been running (necessary for changing between run and stand sprite).

public:
    /**
     * Creates a new Player with the given boxes and members.
     * @param hitBox Hitbox of the Player.
     * @param srcRect Source rectangle for the Texture.
     * @param destRect Destination rectangle for the Texture.
     * @see SDL_Rect
     */
    Player(SDL_Rect hitBox, SDL_Rect srcRect, SDL_Rect destRect);

    /**
     * @brief Updates the player with delta time.
     * @param dt Delta time.
     */
    void Update(float dt);

    /**
     * @brief Renders the player to the screen (renderer).
     */
    void Render();

    /**
     * @brief Resets the player to default state.
     */
    void Reset();

    /**
     * @brief Touches another object.
     * @param object The touched object.
     * @see GameObject
     */
    void Touch(GameObject* object);

    /**
     * @brief Handles being touched by another Entity.
     * @param object The entity that it is touched by.
     * @see Entity
     */
    void TouchedBy(Entity* entity);

    /**
     * @brief Kills the player.
     */
    void Kill();

    /**
     * @brief Changes the player into a giga cat.
     */
    void MakeGiga();

    /**
     * @brief Sets the bottom limit of the end flag
     * @param y Sets the bottom of the end flag (how far it will slide down)
     */
    void SetBottomOfFlag(int y);

    /**
     * @return Returns whether the player is forced by the end flag.
     */
    bool IsForcedByFlag() const;

    /**
     * @return Returns a reference whether the player is on the ground in this update session.
     */
    bool& OnGround();

    /**
     * @return Returns a reference to the spawn point.
     */
    SDL_Rect& SpawnPoint();

    /**
     * @brief Destructor.
     */
    ~Player();
};

/**********************************************************************************************************************************************/

/**
 * @brief Enemy base class inherited from class Entity.
*/
class Enemy : public Entity {
    friend class GameEngine;
protected:
    bool faceLeftDefault;   ///< Contains whether the enemy faces left by default.
    bool isActivated;       ///< Contains whether the enemy is activated.
    int activationPoint;    ///< When the player reaches this x coordinate, the enemy gets activated.
    int shiftTextureRight;  ///< Contains how much the texture is shifted to the right (necessary for rendering the sprite when the enemy faces to the left).

public:
    /**
     * Creates a new Enemy with the given boxes and members.
     * @param hitBox Hitbox of the Enemy.
     * @param activationPoint x coordinate where the enemy is activated.
     * @param srcRect Source rectangle for the Texture.
     * @param shiftTextureRight Contains how much the texture is shifted to the right (necessary for rendering the sprite when the enemy faces to the left).
     * @param destRect Destination rectangle for the Texture.
     * @param faceLeft Contains whether the enemy is facing left.
     * @see SDL_Rect
     */
    Enemy(SDL_Rect hitBox, int activationPoint, SDL_Rect srcRect, int shiftTextureRight, SDL_Rect destRect, bool faceLeft);

    /**
     * @brief Updates the enemy with delta time.
     * @param dt Delta time.
     */
    virtual void Update(float dt);

    /**
     * @brief Renders the enemy to the screen (renderer).
     */
    virtual void Render();

    /**
     * @brief Resets the enemy to default state.
     */
    virtual void Reset();

    /**
     * @brief Touches another object.
     * @param object The touched object.
     * @see GameObject
     */
    virtual void Touch(GameObject* object);

    /**
     * @brief Handles being touched by another Entity.
     * @param object The entity that it is touched by.
     * @see Entity
     */
    virtual void TouchedBy(Entity* entity) = 0;

    /**
     * @brief Kills the enemy.
     */
    virtual void Kill();

    /**
     * @brief Destructor.
     */
    ~Enemy();
};

/**********************************************************************************************************************************************/

/**
 * @brief CommonEnemy class inherited from class Enemy.
*/
class CommonEnemy : public Enemy {
public:
    /**
     * Creates a new CommonEnemy with the given boxes and members.
     * @param hitBox Hitbox of the enemy.
     * @param activationPoint x coordinate where the enemy is activated.
     * @param srcRect Source rectangle for the Texture.
     * @param shiftTextureRight Contains how much the texture is shifted to the right (necessary for rendering the sprite when the enemy faces to the left).
     * @param destRect Destination rectangle for the Texture.
     * @param faceLeft Contains whether the enemy is facing left.
     * @see SDL_Rect
     */
    CommonEnemy(SDL_Rect hitBox, int activationPoint, SDL_Rect srcRect, int shiftTextureRight, SDL_Rect destRect, bool faceLeft);

    /**
     * @brief Handles being touched by another Entity.
     * @param object The entity that it is touched by.
     * @see Entity
     */
    void TouchedBy(Entity* entity);

    /**
     * @brief Destructor.
     */
    ~CommonEnemy();
};

/**********************************************************************************************************************************************/

/**
 * @brief SoldierEnemy class inherited from class Enemy.
*/
class SoldierEnemy : public Enemy {
private:
    bool isSqueezed;    ///< Contains whether the enemy is squeezed.
    bool isSliding;     ///< Contains whether the enemy is sliding.

public:
    /**
     * Creates a new SoldierEnemy with the given boxes and members.
     * @param hitBox Hitbox of the Enemy.
     * @param activationPoint x coordinate where the enemy is activated.
     * @param srcRect Source rectangle for the Texture.
     * @param shiftTextureRight Contains how much the texture is shifted to the right (necessary for rendering the sprite when the enemy faces to the left).
     * @param destRect Destination rectangle for the Texture.
     * @param faceLeft Contains whether the enemy is facing left.
     * @see SDL_Rect
     */
    SoldierEnemy(SDL_Rect hitBox, int activationPoint, SDL_Rect srcRect, int shiftTextureRight, SDL_Rect destRect, bool faceLeft);

    /**
     * @brief Updates the enemy with delta time.
     * @param dt Delta time.
     */
    void Update(float dt) override;

    /**
     * @brief Resets the enemy to default state.
     */
    void Reset() override;

    /**
     * @brief Handles being touched by another Entity.
     * @param object The entity that it is touched by.
     * @see Entity
     */
    void TouchedBy(Entity* entity);

    /**
     * @return Returns whether the enemy is squeezed.
     */
    bool IsSqueezed() const;

    /**
     * @brief Destructor.
     */
    ~SoldierEnemy();
};

/**********************************************************************************************************************************************/

/**
 * @brief KingEnemy class inherited from class Enemy.
*/
class KingEnemy : public Enemy {
public:
    /**
     * Creates a new KingEnemy with the given boxes and members.
     * @param hitBox Hitbox of the Enemy.
     * @param activationPoint x coordinate where the enemy is activated.
     * @param srcRect Source rectangle for the Texture.
     * @param shiftTextureRight Contains how much the texture is shifted to the right (necessary for rendering the sprite when the enemy faces to the left).
     * @param destRect Destination rectangle for the Texture.
     * @param faceLeft Contains whether the enemy is facing left.
     * @see SDL_Rect
     */
    KingEnemy(SDL_Rect hitBox, int activationPoint, SDL_Rect srcRect, int shiftTextureRight, SDL_Rect destRect, bool faceLeft);

    /**
     * @brief Handles being touched by another Entity.
     * @param object The entity that it is touched by.
     * @see Entity
     */
    void TouchedBy(Entity* entity);

    /**
     * @brief Destructor.
     */
    ~KingEnemy();
};

/**********************************************************************************************************************************************/

/**
 * @brief RedMushroomEnemy class inherited from class Enemy.
*/
class RedMushroomEnemy : public Enemy {
public:
    /**
     * Creates a new RedMushroomEnemy with the given boxes and members.
     * @param hitBox Hitbox of the Enemy.
     * @param activationPoint x coordinate where the enemy is activated.
     * @param srcRect Source rectangle for the Texture.
     * @param shiftTextureRight Contains how much the texture is shifted to the right (necessary for rendering the sprite when the enemy faces to the left).
     * @param destRect Destination rectangle for the Texture.
     * @param faceLeft Contains whether the enemy is facing left.
     * @see SDL_Rect
     */
    RedMushroomEnemy(SDL_Rect hitBox, int activationPoint, SDL_Rect srcRect, int shiftTextureRight, SDL_Rect destRect, bool faceLeft);

    /**
     * @brief Handles being touched by another Entity.
     * @param object The entity that it is touched by.
     * @see Entity
     */
    void TouchedBy(Entity* entity);

    /**
     * @brief Destructor.
     */
    ~RedMushroomEnemy();
};

/**********************************************************************************************************************************************/

/**
 * @brief PurpleMushroomEnemy class inherited from class Enemy.
*/
class PurpleMushroomEnemy : public Enemy {
public:
/**
     * Creates a new PurpleMushroomEnemy with the given boxes and members.
     * @param hitBox Hitbox of the Enemy.
     * @param activationPoint x coordinate where the enemy is activated.
     * @param srcRect Source rectangle for the Texture.
     * @param shiftTextureRight Contains how much the texture is shifted to the right (necessary for rendering the sprite when the enemy faces to the left).
     * @param destRect Destination rectangle for the Texture.
     * @param faceLeft Contains whether the enemy is facing left.
     * @see SDL_Rect
     */
    PurpleMushroomEnemy(SDL_Rect hitBox, int activationPoint, SDL_Rect srcRect, int shiftTextureRight, SDL_Rect destRect, bool faceLeft);

    /**
     * @brief Handles being touched by another Entity.
     * @param object The entity that it is touched by.
     * @see Entity
     */
    void TouchedBy(Entity* entity);

    /**
     * @brief Destructor.
     */
    ~PurpleMushroomEnemy();
};

#endif // CPORTA

#endif // ENTITY_H