#ifndef CPORTA

#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>

#endif // CPORTA

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <random>
#include <ctime>

#include "GameEngine.h"
#include "RenderWindow.h"
#include "LanguageModule.h"
#include "Timer.h"
#include "Input.h"
#include "Sound.h"
#include "RigidBody.h"
#include "Entity.h"

/* SDL miatt rengeteg helyen nem lehet const, mert a függvényeknek nem const-ot kell beadni
 * és így "mindenhol" feleselegen kéne const_cast-olni.
 */

int main(int argc, char* arvg[]) {
    #ifndef CPORTA

    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        std::cerr << "SDL_Init has failed. Error: " << SDL_GetError() << std::endl;
        exit(1);
    }

    RenderWindow window("Cat Mario", 1600, 900);
    GameEngine engine(window);
    engine.updateButtons(window); // Because TextButtons are filled up with a space by the constructor, because translation is stroed in ProgramData
    
    const int FPS = 100;
    const int frameDelay = 1000 / FPS;
    Uint32 frameStart;
    int frameTime;

    while (engine.getExitProgram() == false) {
        frameStart = SDL_GetTicks();

        SDL_Event event;
        engine.anyKeyPressed = false;
        // 1. Handling events one-by-one
        while (SDL_PollEvent(&event)) {
            engine.handleEvent(event, window);
        }

        // 2. Update
        engine.handlePressedKeys(window);
        engine.handleSceneChanges(window);

        // 3. Display
        window.clear();
        engine.renderItems(window);
        window.applyTransition(engine.getTransparency());
        window.display();

        // 4. Apply even FPS
        frameTime = SDL_GetTicks() - frameStart;
        // std::cout << "Frametime: " << frameTime << " ms" << std::endl;
        if (frameDelay > frameTime)
            SDL_Delay(frameDelay - frameTime);
    }

    #endif // CPORTA

    std::clog << "***** Exiting Program *****" << std::endl;

    return 0;
}