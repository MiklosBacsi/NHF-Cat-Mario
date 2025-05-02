#ifndef GAME_ENGINE_H
#define GAME_ENGINE_H

#ifndef CPORTA

#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>

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
#include "Animation.h" // ????

#define FPS 100

namespace Scene {
    enum Type { NONE=0, TITLE, MENU, GAME, DEATH };
}

std::string ToString(Scene::Type scene);
std::ostream& operator<<(std::ostream& os, Scene::Type scene);

class GameEngine {
public:
    static RenderWindow* window;
    bool anyKeyPressed;
    const static int frameDelay;
    static int frameTime;
private:
    Transition transition;
    Level* level;
    Level::Type currentLevel;
    Level::Type completedLevels;
    bool exitProgram;
    bool isPaused;
    Scene::Type currentScene;
    Scene::Type nextScene;
    Language currentLanguage;
    std::vector<LanguageModule*> LangMod;
    TextButton* titleButton;
    TextButton* deathButton;
    std::vector<Button*> menuButtons;
    std::vector<Button*> gameButtons;
    Sound sounds;
    Texture titleScreen;
    Texture menuScreen;
    Input input;
    Uint32 frameStart;

    void RenderMenuButtons();
    void RenderGameButtons();
    void RenderPuase();
    void ChangeSceneFromTitleToMenu();
    void ChangeSceneFromMenuToGame(Level::Type levelType);
    void ChangeSceneFromGameToMenu();
    void ChangeSceneFromGameToDeathToGame();
    void HandleEvent(SDL_Event& event);
    void HandleMenuButtons();
    void HandleGameButtons();
    void UpdateSingeButton(Button* button);
    void LoadLevel(Level::Type levelType);
    void ExitProgram();
    void SetLanguage(Language language);
    void SetTransition(size_t miliSeconds=2000);
    void PlaySound(Sound::Type soundType, bool loop=false);
    void StopSounds();
    void LoadSounds();
    Language getLanguage() const;
    bool AABB(const SDL_Rect& A, const SDL_Rect& B) const;
    int OverhangRight(const SDL_Rect& A, const SDL_Rect& B) const;
    int OverhangLeft(const SDL_Rect& A, const SDL_Rect& B) const;
    int OverhangUp(const SDL_Rect& A, const SDL_Rect& B) const;
    int OverhangDown(const SDL_Rect& A, const SDL_Rect& B) const;
    void CheckForDeath();
    void CheckForCollision();
    void RecoverPosition();
    void UpdateRects();
public:
    GameEngine(RenderWindow& window);
    void ApplyEvenFPS();
    void HandleEvents();
    void HandlePressedKeys();
    void HandleSceneChanges();
    void UpdateButtons();
    void UpdateGame();
    void RenderItems();
    void LogScenes() const;
    bool GetExitProgram() const;
    int GetTransparency();
    ~GameEngine();
};

#endif // CPORTA

#endif // GAME_ENGINE_H