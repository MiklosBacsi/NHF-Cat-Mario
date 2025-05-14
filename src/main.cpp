#ifndef CPORTA

#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>

#endif // CPORTA

#include <iostream>
#include <string>

#include "GameEngine.h"
#include "RenderWindow.h"

int main(int argc, char* arvg[]) {
    #ifndef CPORTA

    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        std::cerr << "SDL_Init has failed. Error: " << SDL_GetError() << std::endl;
        exit(1);
    }

    RenderWindow window("Cat Mario", 1600, 900);
    GameEngine engine(window);
    engine.UpdateButtons(); // Because TextButtons are filled up with a space by the constructor, because translation is stroed in ProgramData

    while (engine.GetExitProgram() == false) {
        // 1. Handling events one-by-one
        engine.HandleEvents();

        // 2. Update
        engine.HandlePressedKeys();
        engine.HandleSceneChanges();
        engine.UpdateGame();

        // 3. Display
        window.Clear();
        engine.RenderItems();
        engine.ApplyTransition();
        window.Display();

        // 4. Applys even FPS by delaying
        engine.ApplyEvenFPS();
            
        // std::clog << "Frametime: " << GameEngine::frameTime << " ms" << std::endl;
    }

    #endif // CPORTA

    std::clog << "***** Exiting Program *****" << std::endl;

    return 0;
}