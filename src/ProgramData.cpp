#include <SDL2/SDL.h>
#include <iostream>

#include "ProgramData.h"
#include "RenderWindow.h"
#include "LanguageModule.h"
#include "Input.h"

ProgramData::ProgramData(RenderWindow& window) : nextScene(NONE), isExitProgram(false), isPaused(false),
    currentScene(TITLE), currentLanguage(ENGLISH),
    titleButton (new TextButton(Button::NONE, Lang::PRESS, 610, 800, WHITE, REG30, currentLanguage, window, 100)),
    titleScreen("../res/img/TitleScreen.png", {0, 0, 1600, 900}, window.getRenderer()),
    menuScreen("../res/img/MenuScreen.png", {0, 0, 1600, 900}, window.getRenderer())
    {
    
    LangMod.push_back(new LanguageModule("../res/lang/English.txt"));
    LangMod.push_back(new LanguageModule("../res/lang/Japanese.txt"));
    LangMod.push_back(new LanguageModule("../res/lang/Hungarian.txt"));
    
    menuButtons.push_back((Button*) new TextButton(Button::START, Lang::START, 200, 350, BLACK, MED50, currentLanguage,  window, 100));
    menuButtons.push_back((Button*) new TextButton(Button::NONE, Lang::CAT_MARIO, 60, 80, BLACK, BOLD100, currentLanguage, window, 100));
    menuButtons.push_back((Button*) new TextButton(Button::NONE, Lang::PAUSE, 920, 300, BLACK, REG30, currentLanguage, window, 100));

    menuButtons.push_back((Button*) new ImageButton(Button::ENG, {920, 100, 200, 100}, "../res/img/FlagENG.png", window, true));
    menuButtons.push_back((Button*) new ImageButton(Button::JP, {1170, 100, 150, 100}, "../res/img/FlagJP.png", window));
    menuButtons.push_back((Button*) new ImageButton(Button::HUN, {1370, 100, 150, 100}, "../res/img/FlagHUN.png", window));
    menuButtons.push_back((Button*) new ImageButton(Button::EXIT, {1540, 10, 50, 50}, "../res/img/IconX.png", window));

    gameButtons.push_back((Button*) new ImageButton(Button::EXIT, {1540, 10, 50, 50}, "../res/img/IconX.png", window));
}

void ProgramData::handleEvent(SDL_Event& event, RenderWindow& window) {
    switch (event.type) {
    case SDL_KEYDOWN:
        if (transition.getIsActive())
            return;
        anyKeyPressed = true;

        switch (event.key.keysym.sym) {
        case SDLK_ESCAPE: input.setEsc(true); break;
        case SDLK_w: input.setW(true); break;
        case SDLK_a: input.setA(true); break;
        case SDLK_s: input.setS(true); break;
        case SDLK_d: input.setD(true); break;
        case SDLK_p: input.setP(true); break;
        case SDLK_SPACE: input.setSpace(true); break;
        default: break;
        }
        break;
    case SDL_KEYUP:
        switch (event.key.keysym.sym) {
        case SDLK_ESCAPE: input.setEsc(false); break;
        case SDLK_w: input.setW(false); break;
        case SDLK_a: input.setA(false); break;
        case SDLK_s: input.setS(false); break;
        case SDLK_d: input.setD(false); break;
        case SDLK_p: input.setP(false); break;
        case SDLK_SPACE: input.setSpace(false); break;
        default: break;
        }
        break;
    case SDL_MOUSEBUTTONDOWN:
        if (transition.getIsActive())
            return;
        if (currentScene == TITLE) {
            changeSceneFromTitleToMenu(window);
            return;
        }
        if (event.button.button == SDL_BUTTON_LEFT) {
            input.setMouseClick(true);
            input.setMouseX(event.button.x);
            input.setMouseY(event.button.y);

            switch (currentScene) { // Already handled TITLE
            case MENU: handleMenuButtons(window); break;
            case GAME: handleGameButtons(window); break;
            case DEATH: break;
            default:
                throw "Wrong scene!";
            }
        }
        break;
    case SDL_MOUSEBUTTONUP: if (event.button.button == SDL_BUTTON_LEFT) input.setMouseClick(false); break;
    case SDL_QUIT:
        exitProgram();
        break;
    }
}

void ProgramData::handleSceneChanges(RenderWindow& window) {
    switch (currentScene) {
    case TITLE:
        switch (nextScene) {
        case NONE: break;
        case MENU: changeSceneFromTitleToMenu(window); break;
        default:
            throw "Wrong Scene!";
            break;
        }
        break;
    case MENU:
        switch (nextScene) {
        case MENU: break;
        case GAME: changeSceneFromMenuToGame(window); break;
        default:
            throw "Wrong Scene!";
            break;
        }
        break;
    case GAME:
        switch (nextScene) {
        case GAME: break;
        case DEATH: changeSceneFromGameToDeath(window); break;
        case MENU: changeSceneFromGameToMenu(window); break;
        default:
            throw "Wrong Scene!";
        }
        break;
    case DEATH:
        throw "Not implemented";
        break;
    default:
        throw "Scene not found!";
        break;
    }
}

void ProgramData::handlePressedKeys(RenderWindow& window) {
    if (anyKeyPressed == false)
        return;
    switch (currentScene) {
    case NONE: break;
    case TITLE: changeSceneFromTitleToMenu(window); break;
    case MENU:
        if (input.getEsc())
            exitProgram();
        else if (input.getSpace())
            changeSceneFromMenuToGame(window);
        break;
    case GAME:
        if (input.getPause())
            isPaused = true; // Also handle pause!!!!!!!!!!!!!!!!!
        if (input.getEsc()) {
            if (isPaused)
                isPaused = false; // Also handle continue!!!!!!!!!!!!!!!!!
            else
                changeSceneFromGameToMenu(window);
        }
        // Vertically Still
        if (input.getUp() && input.getDown()) {
            // xxx
        }
        // Up
        else if (input.getUp() == true) {
            // xxx
        }
        // Down
        else if (input.getDown() == true) {
            // xxx
        }

        // Horizontally Still
        if (input.getRight() && input.getLeft()) {
            // xxx
        }
        // Right
        if (input.getRight() == true) {
            // xxx
        }
        // Left
        else if (input.getLeft() == true) {
            // xxx
        }
        break;
    case DEATH: break;
    default: throw "Scene not found!";
    }
}

bool ProgramData::getExitProgram() const { return isExitProgram; }

void ProgramData::exitProgram() { isExitProgram = true; }

Language ProgramData::getLanguage() const { return currentLanguage; }

int ProgramData::getTransparency() { return transition.getTransparency(); }

void ProgramData::setLanguage(Language language) { currentLanguage = language; }

void ProgramData::setTransition(size_t miliSeconds) { transition.setTransition(miliSeconds); }

void ProgramData::renderItems(RenderWindow& window) {
    switch (currentScene) {
    case TITLE:
        if (transition.getPercent() < 0.5f) {
            window.render(titleScreen);
            titleButton->drawButton(window);
        }
        else {
            window.render(menuScreen);
            renderMenuButtons(window);
        }
        break;
    case MENU:
        if (transition.getPercent() < 0.5f) {
            window.render(menuScreen);
            renderMenuButtons(window);
        }
        else {
            window.render(menuScreen);
            renderGameButtons(window);
        }
        break;
    case GAME:
        if (transition.getPercent() < 0.5f) {
            window.render(menuScreen);
            renderGameButtons(window);
        }
        else {
            window.render(menuScreen);
            renderMenuButtons(window);
        }
        break;
    case DEATH:
        break;
    
    default:
        throw "currentScene not found! ProgramData::renderItems()";
    }
}

void ProgramData::renderMenuButtons(RenderWindow& window) {
    for (Button* button : menuButtons)
        button->drawButton(window);
}

void ProgramData::renderGameButtons(RenderWindow& window) {
    for (Button* button : gameButtons)
        button->drawButton(window);
}

void ProgramData::changeSceneFromTitleToMenu(RenderWindow& window) {
    // Already started changes
    if (nextScene == MENU) {
        if (transition.hasExpired()) {
            currentScene = MENU;
            transition.deactivate();
            
            delete titleButton;
            titleButton = nullptr;
        }
        return;
    }
    
    // Handle changes
    nextScene = MENU;
    transition.setTransition(2000);
}

void ProgramData::changeSceneFromMenuToGame(RenderWindow& window) {
    // Already started changes
    if (nextScene == GAME) {
        if (transition.hasExpired()) {
            currentScene = GAME;
            transition.deactivate();
        }
        return;
    }
    
    // Handle changes
    nextScene = GAME;
    transition.setTransition(2000);
}

void ProgramData::changeSceneFromGameToMenu(RenderWindow& window) {
    // Already started changes
    if (nextScene == MENU) {
        if (transition.hasExpired()) {
            currentScene = MENU;
            transition.deactivate();
        }
        return;
    }
    
    // Handle changes
    nextScene = MENU;
    transition.setTransition(2000);
}
void ProgramData::changeSceneFromGameToDeath(RenderWindow& window) {
    // Already started changes
    if (nextScene == DEATH) {
        if (transition.hasExpired()) {
            currentScene = DEATH;
            transition.deactivate();
        }
        return;
    }
    
    // Handle changes
    nextScene = DEATH;
    transition.setTransition(3000);
}
void ProgramData::changeSceneFromDeathToGame(RenderWindow& window) {
    std::cout << "Changing Scene from Death to Game" << std::endl;
}

void ProgramData::handleMenuButtons(RenderWindow& window) {
    for (Button* button : menuButtons) {
        if (button->isClicked(input.getMouseX(), input.getMouseY())) {
            switch (button->getButtonType()) {
            case Button::START: changeSceneFromMenuToGame(window); return;
            case Button::EXIT: exitProgram(); return;
            case Button::ENG: currentLanguage = ENGLISH; updateButtons(window); return;
            case Button::JP: currentLanguage = JAPANESE; updateButtons(window); return;
            case Button::HUN: currentLanguage = HUNGARIAN; updateButtons(window); return;
            case Button::LEV1: loadLevel(); return;
            case Button::LEV2:
                if (1)
                    loadLevel();
                return;
            case Button::NONE: return;
            default:
                std::cout << "Wrong ButtonType: " << button->getButtonType() << std::endl;
                throw "Wrong ButtonType!";
            }
        }
    }
}

void ProgramData::handleGameButtons(RenderWindow& window) {
    for (Button* button : gameButtons) {
        if (button->isClicked(input.getMouseX(), input.getMouseY())) {
            switch (button->getButtonType()) {
            case Button::CONTINUE:
                // Continue
                break;
            case Button::EXIT: changeSceneFromGameToMenu(window); break;
            case Button::ENG: currentLanguage = ENGLISH; updateButtons(window); break;
            case Button::JP: currentLanguage = JAPANESE; updateButtons(window); break;
            case Button::HUN: currentLanguage = HUNGARIAN; updateButtons(window); break;
            case Button::NONE: break;
            default:
                std::cout << "Wrong ButtonType: " << button->getButtonType() << std::endl;
                throw "Wrong ButtonType!";
            }
        }
    }
}

void ProgramData::updateButtons(RenderWindow& window) {
    // Title Screen Button
    if (titleButton != nullptr)
        static_cast<TextButton*>(titleButton)->updateCaption(LangMod[ENGLISH]->getTranslation(
            static_cast<TextButton*>(titleButton)->getCaptionType()), ENGLISH, window);
    // Menu Buttons
    for (Button* button : menuButtons) {
        // Captions
        if (button->getIsTextBased()) {
            switch (currentLanguage) {
            case ENGLISH:
                static_cast<TextButton*>(button)->updateCaption(
                    LangMod[ENGLISH]->getTranslation(static_cast<TextButton*>(button)->getCaptionType()),
                    ENGLISH, window
                );
                static_cast<TextButton*>(button)->destroySelectBoxTexture();
                break;
            case JAPANESE:
                static_cast<TextButton*>(button)->updateCaption(
                    LangMod[JAPANESE]->getTranslation(static_cast<TextButton*>(button)->getCaptionType()),
                    JAPANESE, window
                );
                static_cast<TextButton*>(button)->destroySelectBoxTexture();
                break;
            case HUNGARIAN:
                static_cast<TextButton*>(button)->updateCaption(
                    LangMod[HUNGARIAN]->getTranslation(static_cast<TextButton*>(button)->getCaptionType()),
                    HUNGARIAN, window
                );
                static_cast<TextButton*>(button)->destroySelectBoxTexture();
                break;
            default:
                throw "ButtonType not found!";
            }
        }
        // Language buttons
        else {
            switch (currentLanguage) {
            case ENGLISH: button->setSelected(button->getButtonType() == Button::ENG); break;
            case JAPANESE: button->setSelected(button->getButtonType() == Button::JP); break;
            case HUNGARIAN: button->setSelected(button->getButtonType() == Button::HUN); break;
            default: break;
            }
        }
    }
}

void ProgramData::loadLevel() {
    // Not implemented
}

ProgramData::~ProgramData() {
    if (titleButton != nullptr)
        delete titleButton;
    for (Button* button : menuButtons)
        delete button;
    for (Button* button : gameButtons)
        delete button;
    for (LanguageModule* lang : LangMod)
        delete lang;
    std::cout << "~ProgramData Dtor" << std::endl;
}