#ifndef CPORTA

#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <random>
#include <ctime>

#include "RenderWindow.h"
#include "Texture.h"
#include "LanguageModule.h"
#include "Timer.h"

SDL_Renderer* RenderWindow::renderer = nullptr;

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
    
    #ifdef DTOR
    std::clog << "~Font Dtor" << std::endl;
    #endif
}
/* ************************************************************************************ */

/***** CLASS BUTTON *****/
Button::Button(Button::Type buttonType, SDL_Rect srcRect, SDL_Rect destRect, bool isTextBased, int padding, bool isSelected, int radius)
    : buttonType(buttonType), texture(nullptr, srcRect, destRect), isSelected(isSelected), isTextBased(isTextBased), padding(padding), radius(radius) {
}

bool Button::isClicked(int x, int y) const {
    // Text
    if (isTextBased && x >= texture.getDestX1() - radius && x <= texture.getDestX2() + radius && y >= texture.getDestY1() - radius && y <= texture.getDestY2() + radius)
        return true;
    // Image
    else if (!isTextBased && x >= texture.getDestX1() && x <= texture.getDestX2() && y >= texture.getDestY1() && y <= texture.getDestY2())
        return true;
    return false;
}

bool Button::getIsSelected() const { return isSelected; }

bool Button::getIsTextBased() const { return isTextBased; }

Button::Type Button::getButtonType() const { return buttonType; }

void Button::setSelected(bool selected) {
    isSelected = selected;
    
    // Destroying old selectBox texture
    if (selected == false)
        selectBox.deleteTexture();
}

Button::~Button() {
    #ifdef DTOR
    std::clog << "~Button Dtor" << std::endl;
    #endif
}

TextButton::TextButton(Button::Type buttonType, Lang::CaptionType capType, int x, int y, Colour colour, FontType font, Language language, RenderWindow& window, int bgOpacity, bool isSelected)
    : Button(buttonType, {0,0,0,0}, {x,y,0,0}, true, 5, isSelected, getRadiusFromFont(font)), caption(" "), captionType(capType), surface(nullptr), font(font), colour(colour), backgroundOppacity(bgOpacity) {

    surface = TTF_RenderUTF8_Blended(window.getFont(font, language), " ", getColour(colour)); 
    texture.getTexture() = SDL_CreateTextureFromSurface(RenderWindow::renderer, surface);

    texture.getSrcRect().w = surface->w;
    texture.getSrcRect().h = surface->h;
    texture.getDestRect().w = surface->w;
    texture.getDestRect().h = surface->h;
}

TextButton::TextButton(Button::Type buttonType, std::string caption, int x, int y, Colour colour, FontType font, RenderWindow& window, int bgOpacity, bool isSelected)
    : Button(buttonType, {0,0,0,0}, {x,y,0,0}, true, 5, isSelected, getRadiusFromFont(font)), caption(caption), captionType(Lang::NONE), surface(nullptr), font(font), colour(colour), backgroundOppacity(bgOpacity) {

    surface = TTF_RenderUTF8_Blended(window.getFont(font, ENGLISH), caption.c_str(), getColour(colour));    
    texture.getTexture() = SDL_CreateTextureFromSurface(RenderWindow::renderer, surface);

    texture.getSrcRect().w = surface->w;
    texture.getSrcRect().h = surface->h;
    texture.getDestRect().w = surface->w;
    texture.getDestRect().h = surface->h;
}

ImageButton::ImageButton(Button::Type buttonType, SDL_Rect destRect, const char* path, bool isSelected)
    : Button(buttonType, {0,0,destRect.w,destRect.h}, destRect, false, 5, isSelected) {
    
    texture.loadTexture(path);
}

void TextButton::drawButton(RenderWindow& window) {
    if (backgroundOppacity > 0)
        roundedBoxRGBA(RenderWindow::renderer, texture.getDestX1() - radius, texture.getDestY1() - radius, texture.getDestX2() + radius, texture.getDestY2() + radius, radius, 255, 255, 255, backgroundOppacity);
    
    texture.render();

    if (isSelected)
        drawSelectBox(window);
}

void ImageButton::drawButton(RenderWindow& window) {
    texture.render();
    if (isSelected)
        drawSelectBox(window);
}

void TextButton::drawSelectBox(RenderWindow& window) {
    // Texture already loaded in
    if (selectBox.getTexture() != nullptr) {
        selectBox.render();
        return;
    }

    /*** Loading Texture ***/
    SDL_DestroyTexture(selectBox.getTexture());

    const int r=255, g=215, b=0, a=255;

    int margin = radius - padding;
    if (margin < 3) margin = 3;

    int frameWidth = texture.getDestX2() - texture.getDestX1() + radius * 2 +1;
    int frameHeight = texture.getDestY2() - texture.getDestY1() + radius * 2 +1;

    // Create a transparent target texture
    selectBox.getTexture() = SDL_CreateTexture(RenderWindow::renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, frameWidth, frameHeight);
    SDL_SetTextureBlendMode(selectBox.getTexture(), SDL_BLENDMODE_BLEND);

    // Set render target to our texture
    SDL_SetRenderTarget(RenderWindow::renderer, selectBox.getTexture());
    SDL_SetRenderDrawColor(RenderWindow::renderer, 0, 0, 0, 0); // Transparent
    SDL_RenderClear(RenderWindow::renderer);

    // Draw rounded box on the texture
    roundedBoxRGBA(RenderWindow::renderer, 0, 0, frameWidth - 1, frameHeight - 1, margin, r, g, b, a);

    // Clear center
    SDL_Rect innerRect = { margin, margin, frameWidth - 2 * margin, frameHeight - 2 * margin };
    SDL_SetRenderDrawBlendMode(RenderWindow::renderer, SDL_BLENDMODE_NONE);
    SDL_SetRenderDrawColor(RenderWindow::renderer, 0, 0, 0, 0);
    SDL_RenderFillRect(RenderWindow::renderer, &innerRect);


    // Reset render target to default (screen)
    SDL_SetRenderTarget(RenderWindow::renderer, nullptr);

    // Set destRect and Ready
    selectBox.getDestRect().x = texture.getDestX1() - radius;
    selectBox.getDestRect().y = texture.getDestY1() - radius;
    selectBox.getDestRect().w = frameWidth;
    selectBox.getDestRect().h = frameHeight;

    selectBox.getSrcRect().x = 0;
    selectBox.getSrcRect().y = 0;
    selectBox.getSrcRect().w = frameWidth;
    selectBox.getSrcRect().h = frameHeight;

    // Render Texture
    selectBox.render();
}

void ImageButton::drawSelectBox(RenderWindow& window) {
    // Texture already loaded in
    if (selectBox.getTexture() != nullptr) {
        selectBox.render();
        return;
    }

    /*** Loading Texture ***/
    SDL_DestroyTexture(selectBox.getTexture());

    const int r=255, g=215, b=0, a=255;
    SDL_Renderer* renderer = RenderWindow::renderer;

    int frameWidth = texture.getDestX2() - texture.getDestX1() + padding * 2;
    int frameHeight = texture.getDestY2() - texture.getDestY1() + padding * 2;
    
    // Create a transparent target texture
    selectBox.getTexture() = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, frameWidth, frameHeight);
    SDL_SetTextureBlendMode(selectBox.getTexture(), SDL_BLENDMODE_BLEND);

    // Set render target to our texture
    SDL_SetRenderTarget(renderer, selectBox.getTexture());
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0); // Transparent
    SDL_RenderClear(renderer);

    // Draw rounded box on the texture
    roundedBoxRGBA(renderer, 0, 0, frameWidth - 1, frameHeight - 1, padding, r, g, b, a);

    // Clear center
    SDL_Rect innerRect = { padding, padding, frameWidth - 2 * padding, frameHeight - 2 * padding };
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    SDL_RenderFillRect(renderer, &innerRect);
    

    // Reset render target to default (screen)
    SDL_SetRenderTarget(renderer, nullptr);

    // Set destRect and Ready
    selectBox.getDestRect().x = texture.getDestX1() - padding;
    selectBox.getDestRect().y = texture.getDestY1() - padding;
    selectBox.getDestRect().w = frameWidth;
    selectBox.getDestRect().h = frameHeight;

    selectBox.getSrcRect().x = 0;
    selectBox.getSrcRect().y = 0;
    selectBox.getSrcRect().w = frameWidth;
    selectBox.getSrcRect().h = frameHeight;

    // Render Texture
    selectBox.render();
}

void TextButton::destroySelectBoxTexture() { selectBox.deleteTexture(); }

Lang::CaptionType TextButton::getCaptionType() const { return captionType; }

void TextButton::updateCaption(std::string newCaption, Language newLanguage, RenderWindow& window) {
    caption = newCaption;

    SDL_DestroyTexture(texture.getTexture());
    SDL_FreeSurface(surface);
    surface = nullptr;

    surface = TTF_RenderUTF8_Blended(window.getFont(font, newLanguage), caption.c_str(), getColour(colour));    
    texture.getTexture() = SDL_CreateTextureFromSurface(RenderWindow::renderer, surface);

    texture.getSrcRect().w = surface->w;
    texture.getSrcRect().h = surface->h;
    texture.getDestRect().w = surface->w;
    texture.getDestRect().h = surface->h;
}

TextButton::~TextButton() {
    SDL_FreeSurface(surface);

    #ifdef DTOR
    std::clog << "~TextureButton Dtor" << std::endl;
    #endif
}

ImageButton::~ImageButton() {
    #ifdef DTOR
    std::clog << "~ImageButton Dtor" << std::endl;
    #endif
}
/* ************************************************************************************ */

/***** CLASS RENDER_WINDOW *****/
RenderWindow::RenderWindow(const char* title, int width, int height)
    : window(nullptr), width(width), height(height) {
    // Create SDL Window
    window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, 0);
    if (window == nullptr) {
        std::cerr << "SDL_CreateWindow has failed. Error: " << SDL_GetError() << std::endl;
        exit(1);
    }
    // Create SDL Renderer
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == nullptr) {
        std::cerr << "SDL_CreateRenderer has failed. Error: " << SDL_GetError() << std::endl;
        exit(1);
    }

    Texture::renderer = renderer;

    loadFonts();
}

void RenderWindow::clear() { SDL_RenderClear(renderer); }

void RenderWindow::display() { SDL_RenderPresent(renderer); }

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

void RenderWindow::applyTransition(int transparency) {
    boxRGBA(renderer, 0, 0, width-1, height-1, 0, 0, 0, transparency);
}

void RenderWindow::drawBackground(int r, int g, int b) {
    boxRGBA(renderer, 0, 0, width-1, height-1, r, g, b, 255);
}

void RenderWindow::loadFonts() {
    japaneseFonts.loadFont("../res/font/NotoSansJP-Regular.ttf", 30, REG30);
    latinFonts.loadFont("../res/font/OpenSans-Regular.ttf", 30, REG30);
    
    japaneseFonts.loadFont("../res/font/NotoSansJP-Bold.ttf", 100, BOLD100);
    latinFonts.loadFont("../res/font/OpenSans-Bold.ttf", 100, BOLD100);

    japaneseFonts.loadFont("../res/font/NotoSansJP-Medium.ttf", 50, MED50);
    latinFonts.loadFont("../res/font/OpenSans-Medium.ttf", 50, MED50);

    japaneseFonts.loadFont("../res/font/NotoSansJP-Medium.ttf", 15, MED15);
    latinFonts.loadFont("../res/font/OpenSans-Medium.ttf", 15, MED15);
}

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

RenderWindow::~RenderWindow() {
    if (window != nullptr)
        SDL_DestroyWindow(window);
    if (renderer != nullptr)
        SDL_DestroyRenderer(renderer);
    
    SDL_Quit();
    
    #ifdef DTOR
    std::clog << "~RenderWindow Dtor" << std::endl;
    #endif
}
/* ************************************************************************************ */

/***** CLASS TRANSITION *****/
void Transition::setTransition(size_t miliSeconds) {
    #ifdef QUICK
    timer.activate(200);
    return;
    #endif
    timer.activate(miliSeconds);
    AlreadyReachedMiddle = false;
}

void Transition::deactivate() { timer.deactivate(); }

int Transition::getTransparency() const {
    if (timer.getIsActive() == false)
        return 0;
    float percentage = timer.getPercent();
    if (percentage > 1.0f)
        return 0;

    // Calculating Transparency
    if (percentage < 0.4f)
        return percentage * 637.5f; // 637,5 = 255 / 0,4

    if (percentage > 0.6f)
        return (1.0f - percentage) * 637.5f;

    return 255;
}

float Transition::getPercent() const { return timer.getPercent(); }

bool Transition::getIsActive() const { return timer.getIsActive(); }

bool Transition::hasExpired() const { return timer.hasExpired(); }

bool Transition::isMiddle() const { return AlreadyReachedMiddle == false && timer.getPercent() > 0.5f; }

void Transition::reachMiddle() { AlreadyReachedMiddle = true; }

Transition::~Transition() {
    #ifdef DTOR
    std::clog << "~Transition Dtor" << std::endl;
    #endif
}
/* ************************************************************************************ */

/***** GLOBAL FUNCTIONS *****/

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

int getRadiusFromFont(FontType font) {
    switch (font){
    case REG30:
        return 10;
    case BOLD100:
        return 30;
    case MED50:
        return 15;
    case MED15:
        return 3;
    default:
        throw "Font not found!";
    }
}

#endif // CPORTA