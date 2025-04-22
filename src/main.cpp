#include "SDL2/SDL.h"
#include <iostream>

#include "ProgramData.h"
#include "RenderWindow.h"
#include "LanguageModule.h"

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

    // Title Screen Loop
    Texture titleScreen = window.loadTexture("../res/img/TitleScreen.png", 1600, 900);
    window.clear();
    window.render(titleScreen);
    window.renderText("Press any key to continue...", 610, 800, WHITE, REG30, ENGLISH);
    window.display();
    while (pd.getExitProgram() == false) {
        SDL_Event event;
        SDL_WaitEvent(&event);
        if (event.type == SDL_QUIT)
            pd.exitProgram();
        else if (event.type == SDL_KEYDOWN || event.type == SDL_MOUSEBUTTONDOWN)
            break;
    }

    /* ************************************************************************************ */
    
    // Game Loop
    Texture menuScreen = window.loadTexture("../res/img/MenuScreen.png", 1600, 900);
    
    const int FPS = 100;
    const int frameDelay = 1000 / FPS;
    Uint32 frameStart;
    int frameTime;

    while (pd.getExitProgram() == false) {
        frameStart = SDL_GetTicks();

        SDL_Event event;
        // Handling events one-by-one
        while (SDL_PollEvent(&event)) {
            pd.handleEvent(event, window);
        }
            
        // Beginning of Update
        window.clear();
        window.render(menuScreen);
        pd.renderItems(window);

        // std::string latin_text = "ŐŐűűűűíííéééé";
        // std::string japanse_text = "ダンススターを誕生させるには、自分の中にカオスがなければならない。";
        // window.renderText(latin_text, 30, 100, WHITE, REG30, HUNGARIAN);
        // window.renderText(japanse_text, 30, 200, WHITE, REG30, JAPANESE);

        // End of Update
        
        window.display();

        frameTime = SDL_GetTicks() - frameStart;
        if (frameDelay > frameTime)
            SDL_Delay(frameDelay - frameTime);
    }

    std::cout << "Exiting Program" << std::endl;

    return 0;
}