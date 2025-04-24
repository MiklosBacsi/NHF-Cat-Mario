#include "SDL2/SDL.h"
#include <iostream>

#include "ProgramData.h"
#include "RenderWindow.h"
#include "LanguageModule.h"
#include "Timer.h"

/* SDL miatt rengeteg helyen nem lehet const, mert a függvényeknek nem const-ot kell beadni
 * és így "mindenhol" feleselegen kéne const_cast-olni.
 */

int main(int argc, char* arvg[]) {
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        std::cout << "SDL_Init has failed. Error: " << SDL_GetError() << std::endl;
        exit(1);
    }

    RenderWindow window("Cat Mario", 1600, 900);
    ProgramData pd(window);
    pd.updateButtons(window); // Because TextButtons are filled up with a space by the constructor
    
    const int FPS = 100;
    const int frameDelay = 1000 / FPS;
    Uint32 frameStart;
    int frameTime;

    while (pd.getExitProgram() == false) {
        frameStart = SDL_GetTicks();

        SDL_Event event;
        pd.anyKeyPressed = false;
        // 1. Handling events one-by-one
        while (SDL_PollEvent(&event)) {
            pd.handleEvent(event, window);
        }

        // 2. Update
        pd.handlePressedKeys(window);
        pd.handleSceneChanges(window);

        // 3. Display
        window.clear();
        pd.renderItems(window);
        window.applyTransition(pd.getTransparency());
        window.display();

        // 4. Apply even FPS
        frameTime = SDL_GetTicks() - frameStart;
        // std::cout << "Frametime: " << frameTime << " ms" << std::endl;
        if (frameDelay > frameTime)
            SDL_Delay(frameDelay - frameTime);
    }

    std::cout << "***** Exiting Program *****" << std::endl;

    return 0;
}