#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include <iostream>

#include "RenderWindow.h"
#include "LanguageModule.h"

using std::cout; using std::endl;


/***** CLASS TEXTURE *****/
SDL_Texture* Texture::getTexture() {return texture;}
int Texture::getWidth() const {return width;}
int Texture::getHeight() const {return height;}

Texture::~Texture() {
    SDL_DestroyTexture(texture);
    cout << "~Texture Dtor" << endl;
}
/* ************************************************************************************ */

/***** CLASS FONT *****/
Font::Font() : fonts(FONT_COUNT, nullptr) { TTF_Init(); }

void Font::loadFont(const char* path, int size, FontType fontType) {
    if (fonts[fontType] != nullptr)
        TTF_CloseFont(fonts[fontType]);

    fonts[fontType] = TTF_OpenFont(path, size);
    if (fonts[fontType] == nullptr) {
        SDL_Log("Failed to load font! %s\n", TTF_GetError());
        exit(1);
    }
}

TTF_Font* Font::getFont(FontType font) {
    if (font == FONT_COUNT)
        throw "You are trying to get the count instead of a font! Font::getFont()";
    return fonts.at(font);
}

Font::~Font() {
    for (auto it = fonts.begin(); it != fonts.end(); ++it)
        TTF_CloseFont(*it);
    cout << "~Font Dtor" << endl;
}
/* ************************************************************************************ */

/***** CLASS BUTTON *****/
Button::Button(int x, int y, RenderWindow& window, bool isTextBased, int padding, bool isSelected)
    : srcRect(nullptr), texture(nullptr), isSelected(isSelected), isTextBased(isTextBased), padding(padding) {
}

void Button::drawSelectBox(SDL_Renderer* renderer) {
    const int width = 4;
    const int r=255, g=215, b=0, a=255;
    // TOP
    thickLineRGBA(renderer, srcRect->x - padding, srcRect->y - padding, srcRect->x + srcRect->w + padding, srcRect->y - padding, width, r, g, b, a);
    // BOTTOM
    thickLineRGBA(renderer, srcRect->x - padding, srcRect->y + srcRect->h + padding, srcRect->x + srcRect->w + padding, srcRect->y + srcRect->h + padding, width, r, g, b, a);
    // LEFT
    thickLineRGBA(renderer, srcRect->x - padding, srcRect->y - padding, srcRect->x - padding, srcRect->y + srcRect->h + padding, width, r, g, b, a);
    // RIGHT
    thickLineRGBA(renderer, srcRect->x + srcRect->w + padding, srcRect->y - padding, srcRect->x + srcRect->w + padding, srcRect->y + srcRect->h + padding, width, r, g, b, a);
}

bool Button::isClicked(int x, int y) const {
    if (x >= srcRect->x && x <= srcRect->x + srcRect->w)
        if (y >= srcRect->y && y <= srcRect->y + srcRect->h)
            return true;
    return false;
}

bool Button::getSelected() const { return isSelected; }

void Button::setSelected(bool isSelected) { isSelected = isSelected; }

Button::~Button() {
    SDL_DestroyTexture(texture);
    delete srcRect;
    cout << "~Button Dtor" << endl;
}

TextButton::TextButton(std::string text, int x, int y, Colour colour, FontType font, Language language, RenderWindow& window, int bgOpacity, bool isSelected)
    : Button(x, y, window, true, 5, isSelected), caption(text), surface(nullptr), font(font), colour(colour), backgroundOppacity(bgOpacity) {

    surface = TTF_RenderUTF8_Blended(window.getFont(font, language), text.c_str(), getColour(colour));    
    texture = SDL_CreateTextureFromSurface(window.getRenderer(), surface);

    srcRect = new SDL_Rect;
    srcRect->x = x;
    srcRect->y = y;
    srcRect->w = surface->w;
    srcRect->h = surface->h;
}

ImageButton::ImageButton(int x, int y, const char* path, int width, int height, RenderWindow& window, bool isSelected)
    : Button(x, y, window, false, 2, isSelected) {
    
    texture = IMG_LoadTexture(window.getRenderer(), path);

    if (texture == nullptr)
        cout << "Failed to load texture for Button. Error: " << SDL_GetError() << endl;

    srcRect = new SDL_Rect;
    srcRect->x = x;
    srcRect->y = y;
    srcRect->w = width;
    srcRect->h = height;    
}

void TextButton::drawButton(SDL_Renderer* renderer) {
    if (backgroundOppacity > 0)
        boxRGBA(renderer, srcRect->x - padding, srcRect->y - padding, srcRect->x + srcRect->w + padding,
            srcRect->y + srcRect->h + padding, 255, 255, 255, backgroundOppacity);
    
    SDL_RenderCopy(renderer, texture, NULL, srcRect);
    if (isSelected)
        drawSelectBox(renderer);
}

void ImageButton::drawButton(SDL_Renderer* renderer) {
    SDL_RenderCopy(renderer, texture, NULL, srcRect);
    if (isSelected)
        drawSelectBox(renderer);
}

void TextButton::updateCaption(Language newLanguage, RenderWindow& window) {
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);

    surface = TTF_RenderUTF8_Blended(window.getFont(font, newLanguage), caption.c_str(), getColour(colour));    
    texture = SDL_CreateTextureFromSurface(window.getRenderer(), surface);

    srcRect->w = surface->w;
    srcRect->h = surface->h;
}

TextButton::~TextButton() {
    SDL_FreeSurface(surface);
    cout << "~TextButton Dtor" << endl;
}

ImageButton::~ImageButton() {
    cout << "~ImageButton Dtor" << endl;
}
/* ************************************************************************************ */

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

void RenderWindow::clear() { SDL_RenderClear(renderer); }

void RenderWindow::render(Texture& texture, int destX, int destY) {
    SDL_Rect destination = {destX, destY, texture.getWidth(), texture.getHeight()};
    SDL_RenderCopy(renderer, texture.getTexture(), nullptr, &destination);
}

void RenderWindow::render(Texture& texture, const SDL_Rect* srcRect, const SDL_Rect* destRect) {
    SDL_RenderCopy(renderer, texture.getTexture(), srcRect, destRect);
}

void RenderWindow::display() { SDL_RenderPresent(renderer); }

int RenderWindow::getWidth() const { return width; }

int RenderWindow::getHeight() const { return height; }

SDL_Renderer* RenderWindow::getRenderer() { return renderer; }

TTF_Font* RenderWindow::getFont(FontType font, Language language) {
    switch (language){
    case ENGLISH:
    case HUNGARIAN:
        return latinFonts.getFont(font);
    case JAPANESE:
        return japaneseFonts.getFont(font);
    
    default:
        throw "Language not found! RenderWindow::getFont()";
    }
}

void RenderWindow::renderText(std::string text, int x, int y, Colour colour, FontType font, Language language) {
    SDL_Surface* captionSurface = nullptr;
    switch (language) {
    case ENGLISH:
    case HUNGARIAN:
        captionSurface = TTF_RenderUTF8_Blended(latinFonts.getFont(font), text.c_str(), getColour(colour));
        break;
    case JAPANESE:
        captionSurface = TTF_RenderUTF8_Blended(japaneseFonts.getFont(font), text.c_str(), getColour(colour));
        break;
    
    default:
        throw "Language not found! RenderWindow::renderText()";
    }
    
    SDL_Texture* captionTexture = SDL_CreateTextureFromSurface(renderer, captionSurface);

    SDL_Rect dest = { .x = x, .y = y, .w = captionSurface->w, .h = captionSurface->h };
    SDL_RenderCopy(renderer, captionTexture, NULL, &dest);

    SDL_FreeSurface(captionSurface);
    SDL_DestroyTexture(captionTexture);
}

void RenderWindow::loadFonts() {
    japaneseFonts.loadFont("../res/font/NotoSansJP-Regular.ttf", 30, REG30);
    latinFonts.loadFont("../res/font/OpenSans-Regular.ttf", 30, REG30);
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
/* ************************************************************************************ */

/***** PUBLIC FUNCTIONS *****/

SDL_Color getColour(Colour colour) {
    switch (colour) {
    case WHITE:
        return (SDL_Color) {255, 255, 255};
    case BLACK:
        return (SDL_Color) {0, 0, 0};
    
    default:
        throw "\nColour not found! getColour()\n";
        break;
    }
}