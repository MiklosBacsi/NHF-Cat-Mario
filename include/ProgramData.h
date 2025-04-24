#ifndef PROGRAM_DATA_H
#define PROGRAM_DATA_H

#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <random>
#include <ctime>

#include "RenderWindow.h"
#include "LanguageModule.h"
#include "Timer.h"
#include "Input.h"
#include "Sound.h"

namespace Scene {
    enum Type { NONE=0, TITLE, MENU, GAME, DEATH };
}

std::string toString(Scene::Type scene);
std::ostream& operator<<(std::ostream& os, Scene::Type scene);

class ProgramData {
public:
    bool anyKeyPressed;
private:
    Transition transition;
    Scene::Type nextScene;
    bool isExitProgram;    
    bool isPaused;
    int deathCount;
    Scene::Type currentScene;
    size_t currentCheckpoint;
    Language currentLanguage;
    std::vector<LanguageModule*> LangMod;
    TextButton* titleButton;
    TextButton* deathButton;
    std::vector<Button*> menuButtons;
    std::vector<Button*> gameButtons;
    Texture titleScreen;
    Texture menuScreen;
    Input input;

    void renderMenuButtons(RenderWindow& window);
    void renderGameButtons(RenderWindow& window);
    void renderPuase(RenderWindow& window);
    void changeSceneFromTitleToMenu(RenderWindow& window);
    void changeSceneFromMenuToGame(RenderWindow& window);
    void changeSceneFromGameToMenu(RenderWindow& window);
    void changeSceneFromGameToDeathToGame(RenderWindow& window);
    void handleMenuButtons(RenderWindow& window);
    void handleGameButtons(RenderWindow& window);
    void updateSingeButton(Button* button, RenderWindow& window);
    void loadLevel(); // Which Level, which checkpoint ???
    void exitProgram();
    void setLanguage(Language language);
    void setTransition(size_t miliSeconds=2000);
    Language getLanguage() const;
public:
    ProgramData(RenderWindow& window);
    void handleEvent(SDL_Event& event, RenderWindow& window);
    void handlePressedKeys(RenderWindow& window);
    void handleSceneChanges(RenderWindow& window);
    void updateButtons(RenderWindow& window);
    void renderItems(RenderWindow& window);
    void logScenes() const;
    bool getExitProgram() const;
    int getTransparency();
    ~ProgramData();
};

#endif // PROGRAM_DATA_H