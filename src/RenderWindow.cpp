#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include <iostream>

#include "RenderWindow.h"

using std::cout; using std::endl;

RenderWindow::RenderWindow(const char* title, int width, int height) : window(nullptr), renderer(nullptr) {
    // Create SDL Window
    window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, 0);
    if (window == nullptr) {
        cout << "SDL_CreateWindow has failed. Error: " << SDL_GetError() << endl;
        exit(1);
    }
    // Create SDL Renderer
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == nullptr) {
        cout << "SDL_CreateRenderer has failed. Error: " << SDL_GetError() << endl;
        exit(1);
    }
}