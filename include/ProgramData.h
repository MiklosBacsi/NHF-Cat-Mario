#ifndef PROGRAM_DATA_H
#define PROGRAM_DATA_H

#include <SDL2/SDL.h>
#include <vector>

#include "RenderWindow.h"
#include "LanguageModule.h"

enum sceneType {MENU=0, GAME, DEATH};

class ProgramData {
private:
    bool isExitProgram;    
    bool isPaused;
    sceneType currentScene;
    size_t currentCheckpoint;
    Language currentLanguage;
    std::vector<Button*> menuButtons;
    std::vector<Button*> gameButtons;

    void renderButtons(RenderWindow& window);
public:
    ProgramData(RenderWindow& window);
    bool getExitProgram() const;
    void exitProgram();
    Language getLanguage() const;
    void setLanguage(Language language);
    void renderItems(RenderWindow& window);
    ~ProgramData();
};

#endif // PROGRAM_DATA_H