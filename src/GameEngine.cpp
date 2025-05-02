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

const int GameEngine::frameDelay = 1000 / FPS;
int GameEngine::frameTime = 0;
RenderWindow* GameEngine::window = nullptr;
/* ************************************************************************************ */
/***** Constructor *****/
GameEngine::GameEngine(RenderWindow& window) : level(nullptr), currentLevel(Level::NONE), completedLevels(Level::NONE),
    exitProgram(false), isPaused(false),
    currentScene(Scene::TITLE), nextScene(Scene::NONE), currentLanguage(ENGLISH),
    titleButton (new TextButton(Button::NONE, Lang::PRESS, 610, 810, WHITE, REG30, currentLanguage, 80)),
    deathButton (new TextButton(Button::NONE, "hh", 610, 800, WHITE, REG30)),
    titleScreen("../res/img/TitleScreen.png", {0, 0, 1600, 900}),
    menuScreen("../res/img/MenuScreen.png", {0, 0, 1600, 900})
    {
    GameEngine::window = &window;
    
    LangMod.push_back(new LanguageModule("../res/lang/English.txt"));
    LangMod.push_back(new LanguageModule("../res/lang/Japanese.txt"));
    LangMod.push_back(new LanguageModule("../res/lang/Hungarian.txt"));
    
    menuButtons.push_back((Button*) new TextButton(Button::START, Lang::START, 200, 350, BLACK, MED50, currentLanguage, 200, true));
    menuButtons.push_back((Button*) new TextButton(Button::NONE, Lang::CAT_MARIO, 60, 80, BLACK, BOLD100, currentLanguage, 200));
    menuButtons.push_back((Button*) new TextButton(Button::NONE, Lang::PAUSE, 920, 300, BLACK, REG30, currentLanguage, 200));

    menuButtons.push_back((Button*) new ImageButton(Button::ENG, {920, 100, 200, 100}, "../res/img/FlagENG.png", true));
    menuButtons.push_back((Button*) new ImageButton(Button::JP, {1170, 100, 150, 100}, "../res/img/FlagJP.png"));
    menuButtons.push_back((Button*) new ImageButton(Button::HUN, {1370, 100, 150, 100}, "../res/img/FlagHUN.png"));
    menuButtons.push_back((Button*) new ImageButton(Button::EXIT, {1540, 10, 50, 50}, "../res/img/IconX.png"));

    
    gameButtons.push_back((Button*) new TextButton(Button::CONTINUE, Lang::CONTINUE, 400, 400, BLACK, MED50, currentLanguage, 255));
    gameButtons.push_back((Button*) new TextButton(Button::EXIT, Lang::EXIT_TO_MENU, 400, 550, BLACK, MED50, currentLanguage, 255));
    
    gameButtons.push_back((Button*) new ImageButton(Button::ENG, {690, 200, 200, 100}, "../res/img/FlagENG.png", true));
    gameButtons.push_back((Button*) new ImageButton(Button::JP, {925, 200, 150, 100}, "../res/img/FlagJP.png"));
    gameButtons.push_back((Button*) new ImageButton(Button::HUN, {1110, 200, 150, 100}, "../res/img/FlagHUN.png"));
    gameButtons.push_back((Button*) new ImageButton(Button::CONTINUE, {1325, 175, 50, 50}, "../res/img/IconX.png"));

    LoadSounds();
    PlaySound(Sound::LOBBY, true);
}
/* ************************************************************************************ */

/***** Public Functions *****/
void GameEngine::ApplyEvenFPS() {
    frameTime = SDL_GetTicks() - frameStart;
    
    if (frameDelay > frameTime)
        SDL_Delay(frameDelay - frameTime);
}

void GameEngine::HandleEvents() {
    frameStart = SDL_GetTicks();
    anyKeyPressed = false;
    SDL_Event event;

    while (SDL_PollEvent(&event))
        HandleEvent(event);
}

void GameEngine::HandlePressedKeys() {
    if (anyKeyPressed == false && currentScene != Scene::GAME)
        return;
    switch (currentScene) {
    case Scene::NONE: break;
    case Scene::TITLE: ChangeSceneFromTitleToMenu(); PlaySound(Sound::CLICK); break;
    case Scene::MENU:
        if (input.GetEsc())
            ExitProgram();
        else if (input.GetSpace())
            ChangeSceneFromMenuToGame(Level::LVL1);
        break;
    case Scene::GAME:
        if (input.GetPause())
            isPaused = true; // Also handle pause!!!!!!!!!!!!!!!!!
        if (input.GetEsc()) {
            if (isPaused)
                isPaused = false; // Also handle continue!!!!!!!!!!!!!!!!!
            else
                ChangeSceneFromGameToMenu();
        }
        if (isPaused || nextScene == Scene::MENU)
            return;
        // Vertically Still
        if (input.GetUp() && input.GetDown()) {
            // xxx
        }
        // Up
        else if (input.GetUp() == true) {
            level->player->GetRigidBody().ApplyForceY(-RigidBody::gravity);
        }
        // Down
        else if (input.GetDown() == true) {
            level->player->GetRigidBody().ApplyForceY(0.0f);
        }

        // Horizontally Still
        if ((input.GetRight() && input.GetLeft()) || (!input.GetRight() && !input.GetLeft())) {
            level->player->GetRigidBody().ApplyForceX(0.0f);
        }
        // Right
        else if (input.GetRight() == true) {
            level->player->GetRigidBody().ApplyForceX(2.0f);
        }
        // Left
        else if (input.GetLeft() == true) {
            level->player->GetRigidBody().ApplyForceX(-2.0f);
        }
        break;
    case Scene::DEATH: throw "Scene not allowed!";
    default: throw "Scene not found!";
    }
}

void GameEngine::HandleSceneChanges() {
    switch (currentScene) {
    case Scene::TITLE:
        switch (nextScene) {
        case Scene::NONE: break;
        case Scene::MENU: ChangeSceneFromTitleToMenu(); break;
        default:
            throw "Wrong Scene!";
            break;
        }
        break;
    case Scene::MENU:
        switch (nextScene) {
        case Scene::DEATH: throw "Scene not allowed!";
        case Scene::MENU: break;
        case Scene::GAME: ChangeSceneFromMenuToGame(currentLevel); break;
        default:
            throw "Wrong Scene!";
            break;
        }
        break;
    case Scene::GAME:
        switch (nextScene) {
        case Scene::GAME: break;
        case Scene::DEATH: ChangeSceneFromGameToDeathToGame(); break;
        case Scene::MENU: ChangeSceneFromGameToMenu(); break;
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

void GameEngine::UpdateButtons() {
    // Title Screen Button
    if (titleButton != nullptr)
        static_cast<TextButton*>(titleButton)->UpdateCaption(LangMod[ENGLISH]->GetTranslation(
            static_cast<TextButton*>(titleButton)->GetCaptionType()), ENGLISH);
    
    // Menu Buttons
    for (Button* button : menuButtons)
        UpdateSingeButton(button);

    // Game Buttons
    for (Button* button : gameButtons)
        UpdateSingeButton(button);
}

void GameEngine::UpdateGame() {
    if (level == nullptr || isPaused || currentScene != Scene::GAME || nextScene == Scene::DEATH)
        return;
    
    if (level->player->IsDead())
        ChangeSceneFromGameToDeathToGame();

    level->Update((float)frameDelay);

    CheckForDeath();

    CheckForCollision();

    RecoverPosition(); // if necesary

    UpdateRects();
}

void GameEngine::RenderItems() {
    switch (currentScene) {
    case Scene::TITLE:
        if (transition.GetPercent() < 0.5f) {
            titleScreen.Render();
            titleButton->DrawButton();
        }
        else {
            menuScreen.Render();
            RenderMenuButtons();
        }
        break;
    case Scene::MENU:
        if (transition.GetPercent() < 0.5f) {
            menuScreen.Render();
            RenderMenuButtons();
        }
        else {
            if (level != nullptr)
                level->Render();
        }
        break;
    case Scene::GAME:
        switch (nextScene) {
        // Normal Game
        case Scene::GAME:
            if (level != nullptr)
                level->Render();
            if (isPaused)
                RenderPuase();
            break;
        // Death Scene
        case Scene::DEATH:
        {
            float percentage = transition.GetPercent();
            if (percentage < 0.4f && percentage > 0.6f) {
                window->DrawBackground(0, 0, 0);
                deathButton->DrawButton();
            }
            else {
                if (level != nullptr)
                    level->Render();
            }
        }
            break;
        // Changing back to Menu
        case Scene::MENU:
            if (transition.GetPercent() < 0.5f) {
                if (level != nullptr)
                    level->Render();
            }
            else {
                menuScreen.Render();
                RenderMenuButtons();
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

void GameEngine::LogScenes() const {
    std::clog << "Current: " << currentScene << "\tNext: " << nextScene << std::endl;
}

bool GameEngine::GetExitProgram() const { return exitProgram; }

int GameEngine::GetTransparency() { return transition.GetTransparency(); }
/* ************************************************************************************ */

/***** Private Functions *****/

void GameEngine::RenderMenuButtons() {
    for (Button* button : menuButtons)
        button->DrawButton();
}

void GameEngine::RenderGameButtons() {
    for (Button* button : gameButtons)
        button->DrawButton();
}

void GameEngine::RenderPuase() {
    roundedBoxRGBA(window->GetRenderer(), 200, 150, window->GetWidth()-201, window->GetHeight()-151, 50, 0, 0, 0, 150);
    RenderGameButtons();
}

void GameEngine::ChangeSceneFromTitleToMenu() {
    // Already started changes
    if (nextScene == Scene::MENU) {
        if (transition.HasExpired()) {
            currentScene = Scene::MENU;
            transition.Deactivate();
            
            delete titleButton;
            titleButton = nullptr;
        }
        return;
    }
    
    // Handle changes (Runs only once)
    nextScene = Scene::MENU;
    transition.SetTransition(2000);
}

void GameEngine::ChangeSceneFromMenuToGame(Level::Type levelType) {
    // Already started changes
    if (nextScene == Scene::GAME) {
        if (transition.HasExpired()) {
            currentScene = Scene::GAME;
            transition.Deactivate();
        }
        else if (transition.IsMiddle()) {
            transition.ReachMiddle();
            PlaySound(Sound::BACKGROUND, true);
            LoadLevel(levelType);
        }
        return;
    }
    
    // Handle changes (Runs only once)
    nextScene = Scene::GAME;
    currentLevel = levelType;
    transition.SetTransition(2000);
    StopSounds();
}

void GameEngine::ChangeSceneFromGameToMenu() {
    // Already started changes
    if (nextScene == Scene::MENU) {
        if (transition.HasExpired()) {
            currentScene = Scene::MENU;
            transition.Deactivate();
        }
        else if (transition.IsMiddle()) {
            transition.ReachMiddle();
            PlaySound(Sound::LOBBY, true);
            
            delete level;
            level = nullptr;
        }
        return;
    }
    
    // Handle changes (Runs only once)
    nextScene = Scene::MENU;
    transition.SetTransition(2000);
    StopSounds();
}

void GameEngine::ChangeSceneFromGameToDeathToGame() {
    // Already started changes
    if (nextScene == Scene::DEATH) {
        float percentage = transition.GetPercent();
        // Has Expired
        if (percentage > 1.0f) {
            nextScene = Scene::GAME;
            transition.Deactivate();

            StopSounds();
            PlaySound(Sound::BACKGROUND, true);
        }
        // In the Middle
        else if (transition.IsMiddle()) {
            transition.ReachMiddle();
            
            level->Reset();
        }
        return;
    }
    
    // Handle changes (Runs only once)
    nextScene = Scene::DEATH;
    transition.SetTransition(3000);
    StopSounds();
    PlaySound(Sound::DEATH);
}

void GameEngine::HandleEvent(SDL_Event& event) {
    switch (event.type) {
    case SDL_KEYDOWN:
        if (transition.IsActive())
            return;
        anyKeyPressed = true;

        switch (event.key.keysym.sym) {
        case SDLK_ESCAPE:
            if (currentScene == Scene::GAME)
                PlaySound(Sound::CLICK);
            input.SetEsc(true);
            break;
        case SDLK_w: input.SetW(true); break;
        case SDLK_a: input.SetA(true); break;
        case SDLK_s: input.SetS(true); break;
        case SDLK_d: input.SetD(true); break;
        case SDLK_p:
            if (currentScene == Scene::GAME && !isPaused)
                PlaySound(Sound::CLICK);
            input.SetP(true);
            break;
        case SDLK_SPACE: input.SetSpace(true); break;
        default: break;
        }
        break;
    case SDL_KEYUP:
        switch (event.key.keysym.sym) {
        case SDLK_ESCAPE: input.SetEsc(false); break;
        case SDLK_w: input.SetW(false); break;
        case SDLK_a: input.SetA(false); break;
        case SDLK_s: input.SetS(false); break;
        case SDLK_d: input.SetD(false); break;
        case SDLK_p: input.SetP(false); break;
        case SDLK_SPACE: input.SetSpace(false); break;
        default: break;
        }
        break;
    case SDL_MOUSEBUTTONDOWN:
        if (transition.IsActive())
            return;
        if (currentScene == Scene::TITLE) {
            ChangeSceneFromTitleToMenu();
            PlaySound(Sound::CLICK);
            return;
        }
        if (event.button.button == SDL_BUTTON_LEFT) {
            input.SetMouseClick(true);
            input.SetMouseX(event.button.x);
            input.SetMouseY(event.button.y);

            switch (currentScene) { // Already handled TITLE
            case Scene::MENU: HandleMenuButtons(); break;
            case Scene::GAME: HandleGameButtons(); break;
            case Scene::DEATH: break;
            default:
                throw "Wrong scene!";
            }
        }
        break;
    case SDL_MOUSEBUTTONUP: if (event.button.button == SDL_BUTTON_LEFT) input.SetMouseClick(false); break;
    case SDL_QUIT:
        ExitProgram();
        break;
    }
}

void GameEngine::HandleMenuButtons() {
    for (Button* button : menuButtons) {
        if (button->IsClicked(input.GetMouseX(), input.GetMouseY())) {
            PlaySound(Sound::CLICK);
            switch (button->GetButtonType()) {
            case Button::EXIT: ExitProgram(); return;
            case Button::ENG: currentLanguage = ENGLISH; UpdateButtons(); return;
            case Button::JP: currentLanguage = JAPANESE; UpdateButtons(); return;
            case Button::HUN: currentLanguage = HUNGARIAN; UpdateButtons(); return;
            case Button::START:
            case Button::LEV1: ChangeSceneFromMenuToGame(Level::LVL1); return;
            case Button::LEV2:
                if (completedLevels == Level::NONE)
                    PlaySound(Sound::ERROR);
                else
                    ChangeSceneFromMenuToGame(Level::LVL2);
                return;
            case Button::NONE: return;
            default:
                std::cerr << "Wrong ButtonType: " << button->GetButtonType() << std::endl;
                throw "Wrong ButtonType!";
            }
        }
    }
}

void GameEngine::HandleGameButtons() {
    if (isPaused == false)
        return;
    for (Button* button : gameButtons) {
        if (button->IsClicked(input.GetMouseX(), input.GetMouseY())) {
            PlaySound(Sound::CLICK);
            switch (button->GetButtonType()) {
            case Button::EXIT: ChangeSceneFromGameToMenu(); isPaused = false; break;
            case Button::CONTINUE: isPaused = false; break;
            case Button::ENG: currentLanguage = ENGLISH; UpdateButtons(); break;
            case Button::JP: currentLanguage = JAPANESE; UpdateButtons(); break;
            case Button::HUN: currentLanguage = HUNGARIAN; UpdateButtons(); break;
            case Button::NONE: break;
            default:
                std::cerr << "Wrong ButtonType: " << button->GetButtonType() << std::endl;
                throw "Wrong ButtonType!";
            }
        }
    }
}

void GameEngine::UpdateSingeButton(Button* button) {
    // Captions
    if (button->IsTextBased()) {
        switch (currentLanguage) {
        case ENGLISH:
            static_cast<TextButton*>(button)->UpdateCaption(LangMod[ENGLISH]->GetTranslation(
                static_cast<TextButton*>(button)->GetCaptionType()), ENGLISH);
            static_cast<TextButton*>(button)->DestroySelectBoxTexture();
            break;
        case JAPANESE:
            static_cast<TextButton*>(button)->UpdateCaption(LangMod[JAPANESE]->GetTranslation(
                static_cast<TextButton*>(button)->GetCaptionType()),JAPANESE);
            static_cast<TextButton*>(button)->DestroySelectBoxTexture();
            break;
        case HUNGARIAN:
            static_cast<TextButton*>(button)->UpdateCaption(LangMod[HUNGARIAN]->GetTranslation(
                static_cast<TextButton*>(button)->GetCaptionType()), HUNGARIAN);
            static_cast<TextButton*>(button)->DestroySelectBoxTexture();
            break;
        default:
            throw "ButtonType not found!";
        }
    }

    // Language buttons
    else {
        switch (currentLanguage) {
        case ENGLISH: button->SetSelected(button->GetButtonType() == Button::ENG); break;
        case JAPANESE: button->SetSelected(button->GetButtonType() == Button::JP); break;
        case HUNGARIAN: button->SetSelected(button->GetButtonType() == Button::HUN); break;
        default: break;
        }
    }
}

void GameEngine::LoadLevel(Level::Type levelType) {
    if (level != nullptr)
        delete level;

    switch (levelType) {
    case Level::LVL1: level = new Level("../res/levels/Level1.txt", window); currentLevel = Level::LVL1; break;
    case Level::LVL2: level = new Level("../res/levels/Level1.txt", window); currentLevel = Level::LVL2; break;
    case Level::NONE: throw "Level type not allowed!";
    default: throw "Level not found!";
    }
    if (level == nullptr)
        throw "Failed to load level!";
}

void GameEngine::ExitProgram() { exitProgram = true; }

void GameEngine::SetLanguage(Language language) { currentLanguage = language; }

void GameEngine::SetTransition(size_t miliSeconds) { transition.SetTransition(miliSeconds); }

void GameEngine::PlaySound(Sound::Type soundType, bool loop) { sounds.PlaySound(soundType, loop); }

void GameEngine::StopSounds() { sounds.StopSound(); }

void GameEngine::LoadSounds() {
    sounds.LoadSound("../res/audio/Click.mp3", Sound::CLICK);
    sounds.LoadSound("../res/audio/BackgroundMusic.mp3", Sound::BACKGROUND);
    sounds.LoadSound("../res/audio/Lobby.mp3", Sound::LOBBY);
    sounds.LoadSound("../res/audio/Death.mp3", Sound::DEATH);
    sounds.LoadSound("../res/audio/Error.mp3", Sound::ERROR);
    sounds.LoadSound("../res/audio/Empty.mp3", Sound::EMPTY);
}

Language GameEngine::getLanguage() const { return currentLanguage; }

bool GameEngine::AABB(const SDL_Rect& A, const SDL_Rect& B) const {
    if (A.x + A.w >= B.x && A.x <= B.x + B.w && A.y + A.h >= B.y && A.y <= B.y + B.h)
        return true;
    return false;
}

int GameEngine::OverhangRight(const SDL_Rect& A, const SDL_Rect& B) const {
    return B.x + B.w - A.x;
}

int GameEngine::OverhangLeft(const SDL_Rect& A, const SDL_Rect& B) const {
    return A.x + A.w - B.x;
}

int GameEngine::OverhangUp(const SDL_Rect& A, const SDL_Rect& B) const {
    return A.y + A.h - B.y;
}

int GameEngine::OverhangDown(const SDL_Rect& A, const SDL_Rect& B) const {
    return B.y + B.h - A.y;
}

void GameEngine::CheckForDeath() {
    // Player leaves screen
    if (AABB(level->player->HitBox(), GameObject::screen) == false)
        level->player->Kill();
    // Enemy leaves screen
}

void GameEngine::CheckForCollision() {
    //
}

void GameEngine::RecoverPosition() {
    //
}

void GameEngine::UpdateRects() {
    // Dest Rect
    level->player->UpdateDestRect();
    level->grid.UpdateDestRect();

    // Previous Position
    level->player->UpdatePreviousPosition();
}
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
std::string ToString(Scene::Type scene) {
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
    return os << ToString(scene);
}

#endif // CPORTA