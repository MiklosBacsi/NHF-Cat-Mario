#ifndef PROGRAM_DATA_H
#define PROGRAM_DATA_H

#include <SDL2/SDL.h>
#include <vector>

#include "RenderWindow.h"
#include "LanguageModule.h"
#include "Timer.h"

enum sceneType { NONE=0, TITLE, MENU, GAME, DEATH };

class ProgramData {
private:
    Transition transition;
    sceneType nextScene;
    bool isExitProgram;    
    bool isPaused;
    sceneType currentScene;
    size_t currentCheckpoint;
    Language currentLanguage;
    std::vector<LanguageModule*> LangMod;
    std::vector<Button*> menuButtons;
    std::vector<Button*> gameButtons;
    bool MouseClick;
    int MouseX;
    int MouseY;

    void renderButtons(RenderWindow& window);
    void handleMenuButtons(RenderWindow& window);
    void handleGameButtons(RenderWindow& window);
    void loadLevel(); // Which Level, which checkpoint ???
public:
    ProgramData(RenderWindow& window);
    void handleEvent(SDL_Event& event, RenderWindow& window);
    void updateButtons(RenderWindow& window);
    bool getExitProgram() const;
    void exitProgram();
    Language getLanguage() const;
    int getTransparency();
    void setLanguage(Language language);
    void setTransition(size_t miliSeconds=3000);
    void renderItems(RenderWindow& window);
    ~ProgramData();
};

#endif // PROGRAM_DATA_H