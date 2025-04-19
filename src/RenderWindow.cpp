#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include <iostream>

#include "RenderWindow.h"

using std::cout; using std::endl;

SDL_Texture* Texture::getTexture() {return texture;}
int Texture::getWidth() const {return width;}
int Texture::getHeight() const {return height;}

Texture::~Texture() {
    SDL_DestroyTexture(texture);
    cout << "~Texture Dtor" << endl;
}

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

Texture RenderWindow::loadTexture(const char* filePath, int width, int height) {
    SDL_Texture* texture = nullptr;
    texture = IMG_LoadTexture(renderer, filePath);
    
    if (texture == nullptr)
        cout << "Failed to load texture. Error: " << SDL_GetError() << endl;

    Texture newTexture(texture, width, height);
    return newTexture;
}

void RenderWindow::clear() {SDL_RenderClear(renderer);}

void RenderWindow::render(Texture& texture, int destX, int destY) {
    SDL_Rect destination = {destX, destY, texture.getWidth(), texture.getHeight()};
    SDL_RenderCopy(renderer, texture.getTexture(), nullptr, &destination);
}

void RenderWindow::display() {SDL_RenderPresent(renderer);}

RenderWindow::~RenderWindow() {
    if (window != nullptr)
        SDL_DestroyWindow(window);
    if (renderer != nullptr)
        SDL_DestroyRenderer(renderer);
    
    SDL_Quit();
    cout << "~RenderWindow Dtor" << endl;
}