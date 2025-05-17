/** @file Level.h
  * @brief Contains declaration of a singe game level.
  * 
  * A level contains all the entities, blocks and level elements.
  * 
  * @author Bácsi Miklós
  * @date 2025-05-18
*/

#ifndef LEVEL_H
#define LEVEL_H

#ifndef CPORTA

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <memory>

#include "RenderWindow.h"
#include "GameObject.h"
#include "Texture.h"
#include "RigidBody.h"
#include "Entity.h"
#include "Block.h"
#include "LevelElement.h"
#include "CoinAnimation.h"

#define BLOCK_SIZE 30
#define SCALED_BLOCK_SIZE 75

// Forward declaration
class RenderWindow;

/**
 * @brief Used for level config. Represents position of object or index of grid.
 */
struct Coordinate {
    int x;  ///< Represents x coordinate or row index.
    int y;  ///< Represents y coordinate or column index.
};

/**
 * @brief Used for level config. Contain necessary information to construct an enemy (or certain LevelElement(s)).
 * @see Enemy
 * @see LevelElement
 */
struct EnemyData {
    Coordinate spawnPoint;  ///< Coordinates of spawn point.
    int activationPoint;    ///< When the player reaches this x coordinate, the enemy gets activated.
    bool faceLeft;  ///< Contains whether enemy faces to the left (or up for certain LevelElement(s)).
};

/**
 * @brief A single game level that contains all the entities, block and level elements.
 */
class Level {
    friend class GameEngine;
public:
    enum Type { NONE=0, LVL1, LVL2 };   ///< Type of the level.
    static bool isCompleted;    ///< Contains whether the level has been completed.

private:
    std::unique_ptr<Player> player; ///< Pointer to the player.
    std::vector<std::unique_ptr<Enemy>> enemies;    ///< Stores "default" enemies using heterogeneous collection.
    std::vector<std::unique_ptr<Enemy>> tempEnemies;    ///< Stores enemies created by MysteryBlock(s). Uses heterogeneous collection, and clear the container on every reset.
    Grid grid;  ///< Contains the blocks using heterogeneous collection. Empty blocks are stored as nullptr(s).
    std::vector<std::unique_ptr<LevelElement>> elements;    ///< Stores level elements using heterogeneous collection.
    CoinAnimation animation;    ///< Stores coins used for animation.
    Quote quote;    ///< Stores a quote for the active enemy chosen to display it.
    Enemy* enemyWithQuote;  ///< Pointer to the enemy chosen to have the quote. Value is nullptr if there is no suitable enemy for the role.
    std::unique_ptr<TextButton> quoteButton;    ///< Pointer to the quote's button that a single, chosen enemy has.
    int maxCameraX; ///< Contains the limit of the camera that it cannot move beyond.

    /****************************************************************************************/

    /**
     * @brief Adds a player with the given coordinate.
     * @param x x coordinate of spawn point.
     * @param y y coordinate of spawn point.
     * @see Player
     */
    void AddPlayer(int x, int y);

    /****************************************************************************************/

    /**
     * @brief Adds an upper dirt block with the given grid index.
     * @param row Row index of the grid.
     * @param column Column index of the grid.
     * @see UpperDirtBlock
     */
    void AddUpperDirtBlock(int row, int column);

    /**
     * @brief Adds a lower dirt block with the given grid index.
     * @param row Row index of the grid.
     * @param column Column index of the grid.
     * @see LowerDirtBlock
     */
    void AddLowerDirtBlock(int row, int column);

    /**
     * @brief Adds a brick block with the given grid index.
     * @param row Row index of the grid.
     * @param column Column index of the grid.
     * @see BrickBlock
     */
    void AddBrickBlock(int row, int column);

    /**
     * @brief Adds a boxy block with the given grid index.
     * @param row Row index of the grid.
     * @param column Column index of the grid.
     * @see BoxyBlock
     */
    void AddBoxyBlock(int row, int column);

    /**
     * @brief Adds a hidden block with the given grid index.
     * @param row Row index of the grid.
     * @param column Column index of the grid.
     * @see HiddenBlock
     */
    void AddHiddenBlock(int row, int column);

    /**
     * @brief Adds a mystery block with the given grid index.
     * @param row Row index of the grid.
     * @param column Column index of the grid.
     * @see MysteryBlock
     */
    void AddMysteryBlock(int row, int column);

    /****************************************************************************************/

    /**
     * @brief Adds a common enemy with the given parameters.
     * @param x x coordinate of spawn point.
     * @param y y coordinate of spawn point.
     * @param activationPoint When the player reaches this x coordinate, the enemy gets activated.
     * @param faceLeft Contains whether enemy faces left.
     * @see CommonEnemy
     */
    void AddCommonEnemy(int x, int y, int activationPoint, bool faceLeft);

    /**
     * @brief Adds a soldier enemy with the given parameters.
     * @param x x coordinate of spawn point.
     * @param y y coordinate of spawn point.
     * @param activationPoint When the player reaches this x coordinate, the enemy gets activated.
     * @param faceLeft Contains whether enemy faces left.
     * @see SoldierEnemy
     */
    void AddSoldierEnemy(int x, int y, int activationPoint, bool faceLeft);

    /**
     * @brief Adds a king enemy with the given parameters.
     * @param x x coordinate of spawn point.
     * @param y y coordinate of spawn point.
     * @param activationPoint When the player reaches this x coordinate, the enemy gets activated.
     * @param faceLeft Contains whether enemy faces left.
     * @see KingEnemy
     */
    void AddKingEnemy(int x, int y, int activationPoint, bool faceLeft);

    /**
     * @brief Adds a red mushroom enemy with the given parameters.
     * @param x x coordinate of spawn point.
     * @param y y coordinate of spawn point.
     * @param activationPoint When the player reaches this x coordinate, the enemy gets activated.
     * @param faceLeft Contains whether enemy faces left.
     * @see RedMushroomEnemy
     */
    void AddRedMushroomEnemy(int x, int y, int activationPoint, bool faceLeft);

    /**
     * @brief Adds a purple mushroom enemy with the given parameters.
     * @param x x coordinate of spawn point.
     * @param y y coordinate of spawn point.
     * @param activationPoint When the player reaches this x coordinate, the enemy gets activated.
     * @param faceLeft Contains whether enemy faces left.
     * @see PurpleMushroomEnemy
     */
    void AddPurpleMushroomEnemy(int x, int y, int activationPoint, bool faceLeft);

    /****************************************************************************************/

    /**
     * @brief Adds a hill with the given coordinate.
     * @param x x coordinate.
     * @param y y coordinate.
     * @see Hill
     */
    void AddHill(int x, int y);

    /**
     * @brief Adds a tree with the given coordinate.
     * @param x x coordinate.
     * @param y y coordinate.
     * @see Tree
     */
    void AddTree(int x, int y);

    /**
     * @brief Adds a grass with the given coordinate.
     * @param x x coordinate.
     * @param y y coordinate.
     * @see Grass
     */
    void AddGrass(int x, int y);

    /**
     * @brief Adds a cloud with the given coordinate.
     * @param x x coordinate.
     * @param y y coordinate.
     * @see Cloud
     */
    void AddCloud(int x, int y);

    /**
     * @brief Adds a fish with the given parameters.
     * @param x x coordinate of spawn point.
     * @param y y coordinate of spawn point.
     * @param activationPoint When the player reaches this x coordinate, the fish gets activated.
     * @param faceUp Contains whether the fish faces up.
     * @see Fish
     */
    void AddFish(int x, int y, int activationPoint, bool faceUp);

    /**
     * @brief Adds a laser with the given parameters.
     * @param x x coordinate of spawn point.
     * @param y y coordinate of spawn point.
     * @param activationPoint When the player reaches this x coordinate, the laser gets activated.
     * @param faceLeft Contains whether the laser faces left.
     * @see Laser
     */
    void AddLaser(int x, int y, int activationPoint, bool faceLeft);

    /**
     * @brief Adds a high tube with the given coordinate.
     * @param x x coordinate.
     * @param y y coordinate.
     * @see HighTube
     */
    void AddHighTube(int x, int y);

    /**
     * @brief Adds a middle tube with the given coordinate.
     * @param x x coordinate.
     * @param y y coordinate.
     * @see MiddleTube
     */
    void AddMiddleTube(int x, int y);

    /**
     * @brief Adds a low tube with the given coordinate.
     * @param x x coordinate.
     * @param y y coordinate.
     * @see LowTube
     */
    void AddLowTube(int x, int y);

    /**
     * @brief Adds a checkpoint flag with the given coordinate.
     * @param x x coordinate.
     * @param y y coordinate.
     * @see CheckpointFlag
     */
    void AddCheckpointFlag(int x, int y);

    /**
     * @brief Adds an end flag with the given coordinate.
     * @param x x coordinate.
     * @param y y coordinate.
     * @see EndFlag
     */
    void AddEndFlag(int x, int y);

    /**
     * @brief Adds a house with the given coordinate.
     * @param x x coordinate.
     * @param y y coordinate.
     * @see House
     */
    void AddHouse(int x, int y);

    /****************************************************************************************/

    /**
     * @brief Creates the game objects from the config file.
     * @param configFile Text file that the level is configured from.
     */
    void LoadLevelFromConfigFile(std::string configFile);

    /**
     * @brief Auxiliary function: converts string to bool.
     * @param token Valid arguments: "true" or "false".
     * @return Returns boolean value that the token is converted to.
     */
    bool ParseBool(const std::string& token);

    /**
     * @brief Auxiliary function: reads an integer from input stream.
     * @param in Reference to input file stream.
     * @return Returns the integer read from the input file stream.
     */
    int ReadInt(std::ifstream& in);

    /**
     * @brief Auxiliary function: reads a coordinate consisting of two integers from input stream.
     * @param in Reference to input file stream.
     * @return Returns the coordinate read from the input file stream.
     */
    Coordinate ReadCoordinate(std::ifstream& in);

    /**
     * @brief Auxiliary function: reads an integer from input stream.
     * @param in Reference to input file stream.
     * @return Returns the integer read from the input file stream.
     */
    EnemyData ReadEnemyData(std::ifstream& in);

public:
    /**
     * @brief Creates a level from the config file.
     * @param configFile Path to the config file.
     * @param window Pointer to the window. Necessary for configuration.
     * @param frameDelay Duration between two update cycles.
     */
    Level(std::string configFile, RenderWindow* window, int frameDelay);

    /**
     * @brief Updates all the game objects.
     * @param dt Delta time.
     */
    void Update(float dt);

    /**
     * @brief Renders all the game objects.
     */
    void Render();

    /**
     * @brief Resets all the game objects.
     */
    void Reset();

    /**
     * @brief Destructor. Destroys class Entity's static texture member.
     */
    ~Level();
};

#endif // CPORTA

#endif // LEVEL_H