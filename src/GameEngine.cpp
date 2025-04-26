#ifndef CPORTA

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

#include "GameEngine.h"
#include "RenderWindow.h"
#include "Level.h"
#include "Entity.h"
#include "RigidBody.h"
#include "Animation.h"
#include "Texture.h"
#include "Block.h"
#include "Element.h"
#include "LanguageModule.h"
#include "Timer.h"
#include "Input.h"
#include "Sound.h"

const int GameEngine::frameDelay = 1000 / FPS;
int GameEngine::frameTime = 0;
/* ************************************************************************************ */
/***** Constructor *****/
GameEngine::GameEngine(RenderWindow& window) : isExitProgram(false), isPaused(false),
    currentScene(Scene::TITLE), nextScene(Scene::NONE), currentLanguage(ENGLISH),
    titleButton (new TextButton(Button::NONE, Lang::PRESS, 610, 800, WHITE, REG30, currentLanguage, window, 100)),
    deathButton (new TextButton(Button::NONE, "hh", 610, 800, WHITE, REG30, window)),
    titleScreen("../res/img/TitleScreen.png", {0, 0, 1600, 900}),
    menuScreen("../res/img/MenuScreen.png", {0, 0, 1600, 900})
    {
    
    LangMod.push_back(new LanguageModule("../res/lang/English.txt"));
    LangMod.push_back(new LanguageModule("../res/lang/Japanese.txt"));
    LangMod.push_back(new LanguageModule("../res/lang/Hungarian.txt"));
    
    menuButtons.push_back((Button*) new TextButton(Button::START, Lang::START, 200, 350, BLACK, MED50, currentLanguage,  window, 100, true));
    menuButtons.push_back((Button*) new TextButton(Button::NONE, Lang::CAT_MARIO, 60, 80, BLACK, BOLD100, currentLanguage, window, 100));
    menuButtons.push_back((Button*) new TextButton(Button::NONE, Lang::PAUSE, 920, 300, BLACK, REG30, currentLanguage, window, 100));

    menuButtons.push_back((Button*) new ImageButton(Button::ENG, {920, 100, 200, 100}, "../res/img/FlagENG.png", true));
    menuButtons.push_back((Button*) new ImageButton(Button::JP, {1170, 100, 150, 100}, "../res/img/FlagJP.png"));
    menuButtons.push_back((Button*) new ImageButton(Button::HUN, {1370, 100, 150, 100}, "../res/img/FlagHUN.png"));
    menuButtons.push_back((Button*) new ImageButton(Button::EXIT, {1540, 10, 50, 50}, "../res/img/IconX.png"));

    
    gameButtons.push_back((Button*) new TextButton(Button::CONTINUE, Lang::CONTINUE, 400, 400, BLACK, MED50, currentLanguage,  window, 255));
    gameButtons.push_back((Button*) new TextButton(Button::EXIT, Lang::EXIT_TO_MENU, 400, 550, BLACK, MED50, currentLanguage,  window, 255));
    
    gameButtons.push_back((Button*) new ImageButton(Button::ENG, {690, 200, 200, 100}, "../res/img/FlagENG.png", true));
    gameButtons.push_back((Button*) new ImageButton(Button::JP, {925, 200, 150, 100}, "../res/img/FlagJP.png"));
    gameButtons.push_back((Button*) new ImageButton(Button::HUN, {1110, 200, 150, 100}, "../res/img/FlagHUN.png"));
    gameButtons.push_back((Button*) new ImageButton(Button::CONTINUE, {1325, 175, 50, 50}, "../res/img/IconX.png"));

    loadSounds();
    playSound(Sound::LOBBY, true);
}
/* ************************************************************************************ */

/***** Public Functions *****/
void GameEngine::handleEvent(SDL_Event& event, RenderWindow& window) {
    switch (event.type) {
    case SDL_KEYDOWN:
        if (transition.getIsActive())
            return;
        anyKeyPressed = true;

        switch (event.key.keysym.sym) {
        case SDLK_ESCAPE:
            if (currentScene == Scene::GAME)
                playSound(Sound::CLICK);
            input.setEsc(true);
            break;
        case SDLK_w: input.setW(true); break;
        case SDLK_a: input.setA(true); break;
        case SDLK_s: input.setS(true); break;
        case SDLK_d: input.setD(true); break;
        case SDLK_p:
            if (currentScene == Scene::GAME && !isPaused)
                playSound(Sound::CLICK);
            input.setP(true);
            break;
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
        if (currentScene == Scene::TITLE) {
            changeSceneFromTitleToMenu(window);
            playSound(Sound::CLICK);
            return;
        }
        if (event.button.button == SDL_BUTTON_LEFT) {
            input.setMouseClick(true);
            input.setMouseX(event.button.x);
            input.setMouseY(event.button.y);

            switch (currentScene) { // Already handled TITLE
            case Scene::MENU: handleMenuButtons(window); break;
            case Scene::GAME: handleGameButtons(window); break;
            case Scene::DEATH: break;
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

void GameEngine::handlePressedKeys(RenderWindow& window) {
    if (anyKeyPressed == false)
        return;
    switch (currentScene) {
    case Scene::NONE: break;
    case Scene::TITLE: changeSceneFromTitleToMenu(window); playSound(Sound::CLICK); break;
    case Scene::MENU:
        if (input.getEsc())
            exitProgram();
        else if (input.getSpace())
            changeSceneFromMenuToGame(window);
        break;
    case Scene::GAME:
        if (input.getPause())
            isPaused = true; // Also handle pause!!!!!!!!!!!!!!!!!
        if (input.getEsc()) {
            if (isPaused)
                isPaused = false; // Also handle continue!!!!!!!!!!!!!!!!!
            else
                changeSceneFromGameToMenu(window);
        }
        if (isPaused)
            return;
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
            // Just for testing Death Scene. Don't forget to delete it!
            changeSceneFromGameToDeathToGame(window);
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
    case Scene::DEATH: throw "Scene not allowed!";
    default: throw "Scene not found!";
    }
}

void GameEngine::handleSceneChanges(RenderWindow& window) {
    switch (currentScene) {
    case Scene::TITLE:
        switch (nextScene) {
        case Scene::NONE: break;
        case Scene::MENU: changeSceneFromTitleToMenu(window); break;
        default:
            throw "Wrong Scene!";
            break;
        }
        break;
    case Scene::MENU:
        switch (nextScene) {
        case Scene::MENU: break;
        case Scene::GAME: changeSceneFromMenuToGame(window); break;
        default:
            throw "Wrong Scene!";
            break;
        }
        break;
    case Scene::GAME:
        switch (nextScene) {
        case Scene::GAME: break;
        case Scene::DEATH: changeSceneFromGameToDeathToGame(window); break;
        case Scene::MENU: changeSceneFromGameToMenu(window); break;
        default:
            throw "Wrong Scene!";
        }
        break;
    case Scene::DEATH:
        throw "Not allowed!";
        break;
    default:
        throw "Scene not found!";
        break;
    }
}

void GameEngine::updateButtons(RenderWindow& window) {
    // Title Screen Button
    if (titleButton != nullptr)
        static_cast<TextButton*>(titleButton)->updateCaption(LangMod[ENGLISH]->getTranslation(
            static_cast<TextButton*>(titleButton)->getCaptionType()), ENGLISH, window);
    
    // Menu Buttons
    for (Button* button : menuButtons)
        updateSingeButton(button, window);

    // Game Buttons
    for (Button* button : gameButtons)
        updateSingeButton(button, window);
}

void GameEngine::renderItems(RenderWindow& window) {
    switch (currentScene) {
    case Scene::TITLE:
        if (transition.getPercent() < 0.5f) {
            titleScreen.render();
            titleButton->drawButton(window);
        }
        else {
            menuScreen.render();
            renderMenuButtons(window);
        }
        break;
    case Scene::MENU:
        if (transition.getPercent() < 0.5f) {
            menuScreen.render();
            renderMenuButtons(window);
        }
        else {
            window.drawBackground();
        }
        break;
    case Scene::GAME:
        switch (nextScene) {
        // Normal Game
        case Scene::GAME:
            window.drawBackground();
            if (isPaused)
                renderPuase(window);
            break;
        // Death Scene
        case Scene::DEATH:
        {
            float percentage = transition.getPercent();
            if (percentage < 0.4f && percentage > 0.6f) {
                window.drawBackground(0, 0, 0);
                deathButton->drawButton(window);
            }
            else {
                window.drawBackground();
            }
        }
            break;
        // Changing back to Menu
        case Scene::MENU:
            if (transition.getPercent() < 0.5f) {
                window.drawBackground();
            }
            else {
                menuScreen.render();
                renderMenuButtons(window);
            }
            break;
        default: throw "Wrong Scene!";
        }
        break;
    case Scene::DEATH:
    default:
        throw "currentScene not found! GameEngine::renderItems()";
    }
}

void GameEngine::logScenes() const {
    std::clog << "Current: " << currentScene << "\tNext: " << nextScene << std::endl;
}

bool GameEngine::getExitProgram() const { return isExitProgram; }

int GameEngine::getTransparency() { return transition.getTransparency(); }
/* ************************************************************************************ */

/***** Private Functions *****/

void GameEngine::renderMenuButtons(RenderWindow& window) {
    for (Button* button : menuButtons)
        button->drawButton(window);
}

void GameEngine::renderGameButtons(RenderWindow& window) {
    for (Button* button : gameButtons)
        button->drawButton(window);
}

void GameEngine::renderPuase(RenderWindow& window) {
    roundedBoxRGBA(window.getRenderer(), 200, 150, window.getWidth()-201, window.getHeight()-151, 50, 0, 0, 0, 150);
    renderGameButtons(window);
}

void GameEngine::changeSceneFromTitleToMenu(RenderWindow& window) {
    // Already started changes
    if (nextScene == Scene::MENU) {
        if (transition.hasExpired()) {
            currentScene = Scene::MENU;
            transition.deactivate();
            
            delete titleButton;
            titleButton = nullptr;
        }
        return;
    }
    
    // Handle changes (Runs only once)
    nextScene = Scene::MENU;
    transition.setTransition(2000);
}

void GameEngine::changeSceneFromMenuToGame(RenderWindow& window) {
    // Already started changes
    if (nextScene == Scene::GAME) {
        if (transition.hasExpired()) {
            currentScene = Scene::GAME;
            transition.deactivate();
        }
        else if (transition.isMiddle()) {
            transition.reachMiddle();
            playSound(Sound::BACKGROUND, true);
        }
        return;
    }
    
    // Handle changes (Runs only once)
    nextScene = Scene::GAME;
    transition.setTransition(2000);
    stopSounds();
}

void GameEngine::changeSceneFromGameToMenu(RenderWindow& window) {
    // Already started changes
    if (nextScene == Scene::MENU) {
        if (transition.hasExpired()) {
            currentScene = Scene::MENU;
            transition.deactivate();
        }
        else if (transition.isMiddle()) {
            transition.reachMiddle();
            playSound(Sound::LOBBY, true);
        }
        return;
    }
    
    // Handle changes (Runs only once)
    nextScene = Scene::MENU;
    transition.setTransition(2000);
    stopSounds();
}

void GameEngine::changeSceneFromGameToDeathToGame(RenderWindow& window) {
    // Already started changes
    if (nextScene == Scene::DEATH) {
        float percentage = transition.getPercent();
        // Has Expired
        if (percentage > 1.0f) {
            nextScene = Scene::GAME;
            transition.deactivate();

            stopSounds();
            playSound(Sound::BACKGROUND, true);
        }
        // In the Middle
        else if (transition.isMiddle()) {
            transition.reachMiddle();
            // Reset Level
            // check to only run once
        }
        return;
    }
    
    // Handle changes (Runs only once)
    nextScene = Scene::DEATH;
    transition.setTransition(3000);
    stopSounds();
    playSound(Sound::DEATH);
}

void GameEngine::handleMenuButtons(RenderWindow& window) {
    for (Button* button : menuButtons) {
        if (button->isClicked(input.getMouseX(), input.getMouseY())) {
            playSound(Sound::CLICK);
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
                std::cerr << "Wrong ButtonType: " << button->getButtonType() << std::endl;
                throw "Wrong ButtonType!";
            }
        }
    }
}

void GameEngine::handleGameButtons(RenderWindow& window) {
    if (isPaused == false)
        return;
    for (Button* button : gameButtons) {
        if (button->isClicked(input.getMouseX(), input.getMouseY())) {
            playSound(Sound::CLICK);
            switch (button->getButtonType()) {
            case Button::EXIT: changeSceneFromGameToMenu(window); isPaused = false; break;
            case Button::CONTINUE: isPaused = false; break;
            case Button::ENG: currentLanguage = ENGLISH; updateButtons(window); break;
            case Button::JP: currentLanguage = JAPANESE; updateButtons(window); break;
            case Button::HUN: currentLanguage = HUNGARIAN; updateButtons(window); break;
            case Button::NONE: break;
            default:
                std::cerr << "Wrong ButtonType: " << button->getButtonType() << std::endl;
                throw "Wrong ButtonType!";
            }
        }
    }
}

void GameEngine::updateSingeButton(Button* button, RenderWindow& window) {
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

void GameEngine::loadLevel() {
    // Not implemented
}

void GameEngine::exitProgram() { isExitProgram = true; }

void GameEngine::setLanguage(Language language) { currentLanguage = language; }

void GameEngine::setTransition(size_t miliSeconds) { transition.setTransition(miliSeconds); }

void GameEngine::playSound(Sound::Type soundType, bool loop) { sounds.playSound(soundType, loop); }

void GameEngine::stopSounds() { sounds.stopSound(); }

void GameEngine::loadSounds() {
    sounds.loadSound("../res/audio/Click.mp3", Sound::CLICK);
    sounds.loadSound("../res/audio/BackgroundMusic.mp3", Sound::BACKGROUND);
    sounds.loadSound("../res/audio/Lobby.mp3", Sound::LOBBY);
    sounds.loadSound("../res/audio/Death.mp3", Sound::DEATH);
    sounds.loadSound("../res/audio/Empty.mp3", Sound::EMPTY);
}

Language GameEngine::getLanguage() const { return currentLanguage; }
/* ************************************************************************************ */

/***** Destructor *****/
GameEngine::~GameEngine() {
    if (titleButton != nullptr)
        delete titleButton;
    if (deathButton != nullptr)
        delete deathButton;
    
    for (Button* button : menuButtons)
        delete button;
    for (Button* button : gameButtons)
        delete button;
    
    for (LanguageModule* lang : LangMod)
        delete lang;
    
    
    #ifdef DTOR
    std::clog << "~GameEngine Dtor" << std::endl;
    #endif
}
/* ************************************************************************************ */

/***** Global Functions *****/
std::string toString(Scene::Type scene) {
    switch (scene) {
    case Scene::NONE: return "NONE";
    case Scene::TITLE: return "TITLE";
    case Scene::MENU: return "MENU";
    case Scene::GAME: return "GAME";
    case Scene::DEATH: return "DEATH";
    default: throw "Scene not found!";
    }
}

std::ostream& operator<<(std::ostream& os, Scene::Type scene) {
    return os << toString(scene);
}

#endif // CPORTA