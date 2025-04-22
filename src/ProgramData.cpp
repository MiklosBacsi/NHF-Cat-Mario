#include <SDL2/SDL.h>
#include <iostream>

#include "ProgramData.h"
#include "RenderWindow.h"
#include "LanguageModule.h"

ProgramData::ProgramData(RenderWindow& window) : isExitProgram(false), currentScene(MENU),
    currentLanguage(ENGLISH), MouseClick(false), MouseX(0), MouseY(0) {
    
    menuButtons.push_back((Button*) new TextButton(START, "Start", 100, 200, BLACK, REG30, currentLanguage, window, 100));
    menuButtons.push_back((Button*) new TextButton(NONE, "None", 200, 200, BLACK, REG30, currentLanguage, window, 100, true));

    menuButtons.push_back((Button*) new ImageButton(ENG, {900, 100, 200, 100}, "../res/img/FlagENG.png", window, true));
    menuButtons.push_back((Button*) new ImageButton(JP, {1150, 100, 150, 100}, "../res/img/FlagJP.png", window));
    menuButtons.push_back((Button*) new ImageButton(HUN, {1350, 100, 150, 100}, "../res/img/FlagHUN.png", window));
    menuButtons.push_back((Button*) new ImageButton(EXIT, {1540, 10, 50, 50}, "../res/img/IconX.png", window));
}

void ProgramData::handleEvent(SDL_Event& event, RenderWindow& window) {
    switch (event.type) {
    case SDL_KEYDOWN:
        break;
    case SDL_KEYUP:
        break;
    case SDL_MOUSEBUTTONDOWN:
        if (event.button.button == SDL_BUTTON_LEFT && currentScene != DEATH) {
            MouseClick = true;
            MouseX = event.button.x;
            MouseY = event.button.y;

            switch (currentScene) {
            case MENU:
                handleMenuButtons(window);
                break;
            case GAME:
                handleGameButtons(window);
                break;
            default:
                throw "Wrong scene!";
            }
        }
        break;
    case SDL_MOUSEBUTTONUP:
        if (event.button.button == SDL_BUTTON_LEFT)    
            MouseClick = false;
        break;
    case SDL_MOUSEMOTION:
        if (MouseClick) {
            // Poll Buttons
        }
        MouseX = event.motion.x;
        MouseY = event.motion.y;
        break;
    case SDL_QUIT:
        exitProgram();
        break;
    }
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
            button->drawButton(window);
        break;
    case GAME:
        break;
    default:
        break;
    }
}

void ProgramData::handleMenuButtons(RenderWindow& window) {
    for (Button* button : menuButtons) {
        if (button->isClicked(MouseX, MouseY)) {
            switch (button->getButtonType()) {
            case START:
                loadLevel();
                return;
            case EXIT:
                exitProgram();
                return;
            case ENG:
                currentLanguage = ENGLISH;    
                updateButtons(window);
                return;
            case JP:
                currentLanguage = JAPANESE;
                updateButtons(window);
                return;
            case HUN:
                currentLanguage = HUNGARIAN;
                updateButtons(window);
                return;
            case LEV1:
                loadLevel();
                return;
            case LEV2:
                if (1)
                    loadLevel();
                return;
            case NONE:
                return;
            default:
                std::cout << "Wrong ButtonType: " << button->getButtonType() << std::endl;
                throw "Wrong ButtonType!";
            }
        }
    }
}

void ProgramData::handleGameButtons(RenderWindow& window) {
    throw "Not implemented!";
}

void ProgramData::updateButtons(RenderWindow& window) {
    for (Button* button : menuButtons) {
        // Captions
        if (button->getIsTextBased()) {
            switch (currentLanguage) {
            case ENGLISH:
                static_cast<TextButton*>(button)->updateCaption("Dick", ENGLISH, window);
                break;
            case JAPANESE:
                static_cast<TextButton*>(button)->updateCaption("肉棒", JAPANESE, window);
                break;
            case HUNGARIAN:
                static_cast<TextButton*>(button)->updateCaption("Fasz", HUNGARIAN, window);
                break;
            default:
                throw "ButtonType not found!";
            }
        }
        // Language buttons
        else {
            switch (currentLanguage) {
            case ENGLISH:
                button->setSelected(button->getButtonType() == ENG);
                break;
            case JAPANESE:
                button->setSelected(button->getButtonType() == JP);
                break;
            case HUNGARIAN:
                button->setSelected(button->getButtonType() == HUN);
                break;
            default:
                break;
            }
        }
    }
}

void ProgramData::loadLevel() {
    // Not implemented
}

ProgramData::~ProgramData() {
    for (Button* button : menuButtons)
        delete button;
    for (Button* button : gameButtons)
        delete button;
    std::cout << "~ProgramData Dtor" << std::endl;
}