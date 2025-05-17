/** @file main.cpp
  * @brief 2D platformer game named Cat Mario
  *  
  * This is the main file of the game, calling the major functions in order.
  * 
  * It initialises the window and the game engine as well, and contains the
  * title screen and game loop too.
  * 
  * This program uses the SDL2 library for graphical representation, playing
  * audio, and handling inputs, and unfortunately, there is a known memory
  * leak in this library.
  *
  * @author Bácsi Miklós
  * @date 2025-05-17
*/

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

int main(int argc, char* argv[]) {
    #ifndef CPORTA

    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        std::cerr << "SDL_Init has failed. Error: " << SDL_GetError() << std::endl;
        exit(1);
    }

    RenderWindow window("Cat Mario", 1600, 900);
    GameEngine engine(window);

    // This function is called, because TextButtons are filled up with a space
    // by the constructor and the translation is stored in GameEngine.
    engine.UpdateButtons();

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