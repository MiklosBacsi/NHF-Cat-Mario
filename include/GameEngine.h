/** @file GameEngine.h
  * @brief Contains class GameEngine which manages all the interactions of the program.
  * @author Bácsi Miklós
  * @date 2025-05-18
*/

#ifndef GAME_ENGINE_H
#define GAME_ENGINE_H

#ifndef CPORTA

#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>

#include <memory>
#include <iostream>
#include <string>
#include <vector>

#include "RenderWindow.h"
#include "Level.h"
#include "GameObject.h"
#include "Texture.h"
#include "RigidBody.h"
#include "Entity.h"
#include "Block.h"
#include "LevelElement.h"
#include "LanguageModule.h"
#include "Timer.h"
#include "Input.h"
#include "Sound.h"

#define FPS 100

namespace Scene {
    enum Type { NONE=0, TITLE, MENU, GAME, DEATH, LOAD };   ///< Type of the Scene.
}

/**
 * @brief Converts a Scene type to string.
 * @param scene Type of the scene.
 * @return Returns a string
 */
std::string ToString(Scene::Type scene);

/**
 * @brief Prints scene type to the output stream.
 * @param scene Type of the scene.
 * @return Returns the output stream as a reference.
 */
std::ostream& operator<<(std::ostream& os, Scene::Type scene);

/**
 * @brief It manages all the interactions of the program.
 */
class GameEngine {
public:
    static RenderWindow* window;    ///< Static member, pointer to the window.
    const static int frameDelay;    ///< Static member, contains the frame delay.
    static int frameTime;   ///< Static member, contains the frameTime.
private:
    bool anyKeyPressed; ///< Contains whether any keys were pressed in a single update session.
    Transition transition;      ///< Contains transition module for transitions between scenes.
    std::unique_ptr<Level> level;   ///< Pointer to the loaded level (nullptr if no level is loaded).
    Level::Type currentLevel;   ///< Contains the type of the current level.
    Level::Type nextLevel;      ///< Contains the type of the next level.
    Level::Type completedLevels;    ///< Contains the maximum completed levels.
    bool exitProgram;   ///< Contains whether the program is exited.
    bool isPaused;      ///< Contains whether the program is paused.
    Scene::Type currentScene;   ///< Contains the type of the current scene.
    Scene::Type nextScene;      ///< Contains the type of the next scene.
    Language currentLanguage;   ///< Contains the type of the current language.
    std::vector<LanguageModule*> LangMod;   ///< Contains the loaded language modules.
    TextButton* titleButton;    ///< Title button (caption).
    TextButton* deathButton;    ///< Death button (counter).
    std::vector<Button*> menuButtons;   ///< Contains all the menu buttons.
    std::vector<Button*> gameButtons;   ///< Contains all the game buttons.
    Sound sounds;   ///< Contains all the loaded sounds.
    Texture titleScreen;    ///< Contains the texture of the title screen.
    Texture menuScreen;     ///< Contains the texture of the menu screen.
    Input input;    ///< Contains the input booleans.
    Uint32 frameStart;  ///< Contains the frame start of each update session.

    /****************************************************************************************/

    /**
     * @brief Renders the buttons in menu scene.
     */
    void RenderMenuButtons();

    /**
     * @brief Renders the buttons in game scene.
     */
    void RenderGameButtons();

    /**
     * @brief Renders backdrop and game buttons. Called when game is paused.
     */
    void RenderPause();

    /****************************************************************************************/

    /**
     * @brief Handles scene changes from title screen to menu.
     */
    void ChangeSceneFromTitleToMenu();

    /**
     * @brief Handles scene changes from menu to game.
     */
    void ChangeSceneFromMenuToGame();

    /**
     * @brief Handles scene changes from game to menu.
     */
    void ChangeSceneFromGameToMenu();

    /**
     * @brief Handles scene changes from game scene to death scene and back to game scene.
     */
    void ChangeSceneFromGameToDeathToGame();

    /**
     * @brief Handles scene changes to a different level, when a level is completed.
     */
    void ChangeSceneToNextLevel();

    /****************************************************************************************/

    /**
     * @brief Handles a single SLD event
     * @param event Reference to the event.
     * @see SDL_Event
     */
    void HandleEvent(SDL_Event& event);

    /**
     * @brief Handles pressed buttons in scene menu.
     */
    void HandleMenuButtons();

    /**
     * @brief Handles pressed buttons in scene game.
     */
    void HandleGameButtons();

    /**
     * @brief Updates a single button's attributes. Calls the function that translates to a new language and changes texture.
     * @param button Pointer to the button which is to be updated.
     */
    void UpdateSingeButton(Button* button);

    /****************************************************************************************/

    /**
     * @brief Loads the (next) level.
     * @see nextLevel
     */
    void LoadLevel();

    /**
     * @brief Sets exit program boolean to true.
     */
    void ExitProgram();

    /**
     * @brief Sets the current language to the given one.
     * @param language Language that we change to.
     * @see Language
     */
    void SetLanguage(Language language);

    /**
     * @brief Initiates transition post-processing between scenes.
     * @param milliSeconds How long the transition shall take, given in milliseconds.
     * @see Transition
     */
    void SetTransition(size_t milliSeconds=2000);

    /****************************************************************************************/

    /**
     * @brief Plays a sound of the given type.
     * @param soundType Type of the sound.
     * @param loop Gives whether the sound is to be looped.
     * @see Sound
     */
    void PlaySound(Sound::Type soundType, bool loop=false);

    /**
     * @brief Stops all playing sounds.
     * @see Sound
     */
    void StopSounds();

    /**
     * @brief Loads all the necessary sounds for the program.
     * @see Sound
     */
    void LoadSounds();

    /**
     * @brief Returns the current language.
     * @see Language
     */
    Language getLanguage() const;

    /****************************************************************************************/

    /**
     * @brief Checks if the level is completed, and if so, it changes to the appropriate scene.
     * @return Returns whether the level is completed.
     */
    bool CheckIfLevelCompleted();

    /**
     * @brief Checks whether a game object dies.
     */
    void CheckForDeath();

    /**
     * @brief Checks whether an entity collides with a game object.
     */
    void CheckForCollision();

    /**
     * @brief Checks whether an animations need to be played.
     */
    void CheckForAnimation();

    /****************************************************************************************/

    /**
     * @brief Assigns a quote to the leftmost enemy.
     */
    void AssignQuote();

    /**
     * @brief Recovers the player's position if its collision cannot be handled properly.
     */
    void RecoverPosition();

    /**
     * @brief Updates all the game objects' rectangles. Necessary for rendering them in the correct place on the screen.
     */
    void UpdateRects();

    /**
     * @brief Draws death count in death scene.
     */
    void DrawDeathCount();

    /**
     * @return Returns the current transparency of the transition (post-processing).
     */
    int GetTransparency();

public:
    /**
     * @brief Creates a game engine and initialises its components.
     * @param window Reference to the window of the program. Necessary for accessing window and its components.
     * @see RenderWindow
     */
    GameEngine(RenderWindow& window);

    /**
     * @brief Ensures even FPS and update frequency.
     */
    void ApplyEvenFPS();

    /**
     * @brief Handles all the events one-by-one.
     * @see SDL_Event
     */
    void HandleEvents();

    /**
     * @brief Manages what happens when the given keys are pressed and on which scenes.
     */
    void HandlePressedKeys();

    /**
     * @brief Manages the changing between scenes.
     */
    void HandleSceneChanges();

    /**
     * @brief Updates all the buttons on all scenes. Usually called when selecting a different language.
     */
    void UpdateButtons();

    /**
     * @brief Manages one update cycle of the game including: updating position, resolving collision, and checking for death.
     */
    void UpdateGame();

    /**
     * @brief Renders the appropiate items to the screen (renderer).
     */
    void RenderItems();

    /**
     * @brief Applies transition between scenes. Works basically as post-processing.
     */
    void ApplyTransition();

    /**
     * @brief Logs the current and the next scene to stream std::clog.
     */
    void LogScenes() const;

    /**
     * @return Returns whether the game has been exited.
     */
    bool GetExitProgram() const;

    /**
     * @brief Destructor: deletes buttons and the language modules.
     */
    ~GameEngine();
};

#endif // CPORTA

#endif // GAME_ENGINE_H