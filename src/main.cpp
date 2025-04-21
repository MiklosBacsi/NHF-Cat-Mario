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
    bool exitProgram = false;

    // Title Screen Loop
    Texture titleScreen = window.loadTexture("../res/img/titleScreen.png", 1600, 900);
    window.clear();
    window.render(titleScreen);
    window.display();
    while (!exitProgram) {
        SDL_Event event;
        SDL_WaitEvent(&event);
        if (event.type == SDL_QUIT)
            exitProgram = true;
        else if (event.type == SDL_KEYDOWN)
            break;
    }
    
    // Game Loop
    Texture menuScreen = window.loadTexture("../res/img/game.png", 1600, 900);
    while (!exitProgram) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT)
                exitProgram = true;

            window.clear();
            
            // code

            window.render(menuScreen);

            std::string latin_text = "ŐŐűűűűíííéééé";
            std::string japanse_text = "ダンススターを誕生させるには、自分の中にカオスがなければならない。";
            window.renderText(latin_text, 30, 100, WHITE, REG30, HUNGARIAN);
            window.renderText(japanse_text, 30, 200, WHITE, REG30, JAPANESE);

            window.display();
        }
    }

    std::cout << "Exiting Program" << std::endl;

    return 0;
}