#include <SDL2/SDL.h>
#include <iostream>

#include "ProgramData.h"
#include "RenderWindow.h"
#include "LanguageModule.h"

ProgramData::ProgramData(RenderWindow& window) : isExitProgram(false), currentScene(MENU),
    currentLanguage(ENGLISH) {
    
    menuButtons.push_back((Button*) new TextButton("Start", 100, 200, BLACK, REG30, currentLanguage, window, 100));
    menuButtons.push_back((Button*) new TextButton("Exit", 200, 200, BLACK, REG30, currentLanguage, window, 100, true));

    menuButtons.push_back((Button*) new ImageButton(900, 100, "../res/img/FlagENG.png", 200, 100, window, true));
    menuButtons.push_back((Button*) new ImageButton(1150, 100, "../res/img/FlagJP.png", 150, 100, window));
    menuButtons.push_back((Button*) new ImageButton(1350, 100, "../res/img/FlagHUN.png", 150, 100, window));
}

bool ProgramData::getExitProgram() const { return isExitProgram; }

void ProgramData::exitProgram() { isExitProgram = true; }

Language ProgramData::getLanguage() const { return currentLanguage; }

void ProgramData::setLanguage(Language language) { currentLanguage = language; }

void ProgramData::renderItems(RenderWindow& window) {
    switch (currentScene) {
    case MENU:
        renderButtons(window);
        break;
    case GAME:
        break;
    case DEATH:
        break;
    
    default:
        throw "currentScene not found! ProgramData::renderItems()";
    }
}

void ProgramData::renderButtons(RenderWindow& window) {
    switch (currentScene) {
    case MENU:
        for (Button* button : menuButtons)
            button->drawButton(window.getRenderer());
        break;
    case GAME:
        break;
    default:
        break;
    }
}

ProgramData::~ProgramData() {
    for (Button* button : menuButtons)
        delete button;
    for (Button* button : gameButtons)
        delete button;
    std::cout << "~ProgramData Dtor" << std::endl;
}