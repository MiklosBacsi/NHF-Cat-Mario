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
    
    Uint32 frameStart;

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
        GameEngine::frameTime = SDL_GetTicks() - frameStart;
        if (GameEngine::frameDelay > GameEngine::frameTime)
            SDL_Delay(GameEngine::frameDelay - GameEngine::frameTime);
            
        // std::clog << "Frametime: " << GameEngine::frameTime << " ms" << std::endl;
    }

    #endif // CPORTA

    std::clog << "***** Exiting Program *****" << std::endl;

    return 0;
}