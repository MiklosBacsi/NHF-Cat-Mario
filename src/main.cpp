#include "SDL2/SDL.h"
#include <iostream>

#include "RenderWindow.h"

using std::cout; using std::endl;

int main(int argc, char* arvg[]) {
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        cout << "SDL_Init has failed. Error: " << SDL_GetError() << endl;
        exit(1);
    }
        
    RenderWindow window;

    SDL_Event ev;
    while (SDL_WaitEvent(&ev) && ev.type != SDL_QUIT) {}
    
    SDL_Quit();

    return 0;
}