#include "SDL2/SDL.h"
#include <iostream>

#include "RenderWindow.h"

using std::cout; using std::endl;

int main(int argc, char* arvg[]) {
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        cout << "SDL_Init has failed. Error: " << SDL_GetError() << endl;
        exit(1);
    }
        
    RenderWindow window("Cat Mario", 1600, 900);

    SDL_Event event;

    bool exitProgram = false;

    // Title Screen Loop
    Texture titleScreen = window.loadTexture("../res/img/titleScreen.png", 1600, 900);
    window.clear();
    window.render(titleScreen);
    window.display();
    while (SDL_WaitEvent(&event) && !exitProgram) {
        if (event.type == SDL_QUIT)
            exitProgram = true;
        else if (event.type == SDL_KEYDOWN)
            break;
    }
    
    // Game Loop
    Texture gameScreenTest = window.loadTexture("../res/img/game.png", 1600, 900);
    window.clear();
    window.render(gameScreenTest);
    while (!exitProgram) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT)
                exitProgram = true;
            window.display();
            // code
        }
    }
    
    SDL_Quit();

    return 0;
}