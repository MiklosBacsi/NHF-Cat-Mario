#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include <iostream>

#include "RenderWindow.h"

using std::cout; using std::endl;


/***** CLASS TEXTURE *****/
SDL_Texture* Texture::getTexture() {return texture;}
int Texture::getWidth() const {return width;}
int Texture::getHeight() const {return height;}

Texture::~Texture() {
    SDL_DestroyTexture(texture);
    cout << "~Texture Dtor" << endl;
}


/***** CLASS FONT *****/
Font::Font() : fonts(FONT_COUNT, nullptr) {
    TTF_Init();
}

void Font::loadFont(const char* path, int size, Fonts fontType) {
    if (fonts[fontType] != nullptr)
        TTF_CloseFont(fonts[fontType]);

    fonts[fontType] = TTF_OpenFont(path, size);
    if (fonts[fontType] == nullptr) {
        SDL_Log("Failed to load font! %s\n", TTF_GetError());
        exit(1);
    }
}

TTF_Font* Font::getFont(Fonts font) {
    if (font == FONT_COUNT)
        throw "You are trying to get the count instead of a font! Font::getFont()";
    return fonts.at(font);
}

Font::~Font() {
    for (auto it = fonts.begin(); it != fonts.end(); ++it)
        TTF_CloseFont(*it);
    cout << "~Font Dtor" << endl;
}


/***** CLASS RENDER_WINDOW *****/
RenderWindow::RenderWindow(const char* title, int width, int height)
    : window(nullptr), renderer(nullptr), width(width), height(height) {
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

    loadFonts();
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

int RenderWindow::getWidth() const {return width;}

int RenderWindow::getHeight() const {return height;}

void RenderWindow::renderText(std::string text, int x, int y, Colour colour, Fonts font) {
    SDL_Surface* captionSurface = TTF_RenderUTF8_Blended(fonts.getFont(font), text.c_str(), getColour(colour));
    SDL_Texture* captionTexture = SDL_CreateTextureFromSurface(renderer, captionSurface);

    SDL_Rect dest = { .x = x, .y = y, .w = captionSurface->w, .h = captionSurface->h };
    SDL_RenderCopy(renderer, captionTexture, NULL, &dest);

    SDL_FreeSurface(captionSurface);
    SDL_DestroyTexture(captionTexture);
}

SDL_Color RenderWindow::getColour(Colour colour) {
    switch (colour) {
    case WHITE:
        return (SDL_Color) {255, 255, 255};
    case BLACK:
        return (SDL_Color) {0, 0, 0};
    
    default:
        throw "\nColour not found! RenderWindow::getColour()\n";
        break;
    }
}

TTF_Font* RenderWindow::getFont(Fonts font) {
    return fonts.getFont(font);
}

void RenderWindow::loadFonts() {
    fonts.loadFont("../res/font/NotoSansJP-Regular.ttf", 30, REG30);
    cout << "Ne felejtsd el betölteni a betűtípusokat!!!" << endl;
}

RenderWindow::~RenderWindow() {
    if (window != nullptr)
        SDL_DestroyWindow(window);
    if (renderer != nullptr)
        SDL_DestroyRenderer(renderer);
    
    SDL_Quit();
    cout << "~RenderWindow Dtor" << endl;
}