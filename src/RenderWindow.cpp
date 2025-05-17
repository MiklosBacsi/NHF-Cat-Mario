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

void Font::LoadFont(const char* path, int size, FontType fontType) {
    if (fonts[fontType] != nullptr)
        TTF_CloseFont(fonts[fontType]);

    fonts[fontType] = TTF_OpenFont(path, size);
    if (fonts[fontType] == nullptr) {
        SDL_Log("Failed to load font! %s\n", TTF_GetError());
        exit(1);
    }
}

TTF_Font* Font::GetFont(FontType font) {
    if (font == FONT_COUNT)
        throw "You are trying to get the count instead of a font! Font::GetFont()";
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
RenderWindow* Button::window = nullptr;
Button::Button(Button::Type buttonType, SDL_Rect srcRect, SDL_Rect destRect, bool isTextBased, int padding, bool isSelected, int radius)
    : buttonType(buttonType), texture(nullptr, srcRect, destRect), isSelected(isSelected), isTextBased(isTextBased), padding(padding), radius(radius) {
}

bool Button::IsClicked(int x, int y) const {
    // Text
    if (isTextBased && x >= texture.GetDestX1() - radius && x <= texture.GetDestX2() + radius && y >= texture.GetDestY1() - radius && y <= texture.GetDestY2() + radius)
        return true;
    // Image
    else if (!isTextBased && x >= texture.GetDestX1() && x <= texture.GetDestX2() && y >= texture.GetDestY1() && y <= texture.GetDestY2())
        return true;
    return false;
}

bool Button::IsSelected() const { return isSelected; }

bool Button::IsTextBased() const { return isTextBased; }

Button::Type Button::GetButtonType() const { return buttonType; }

void Button::SetSelected(bool selected) {
    isSelected = selected;
    
    // Destroying old selectBox texture
    if (selected == false)
        selectBox.DeleteTexture();
}

Button::~Button() {
    #ifdef DTOR
    std::clog << "~Button Dtor" << std::endl;
    #endif
}

TextButton::TextButton(Button::Type buttonType, Lang::CaptionType capType, int x, int y, Colour colour, FontType font, Language language, int bgOpacity, bool isSelected)
    : Button(buttonType, {0,0,0,0}, {x,y,0,0}, true, 5, isSelected, GetRadiusFromFont(font)), caption(" "), captionType(capType), surface(nullptr), font(font), colour(colour), backgroundOpacity(bgOpacity) {

    surface = TTF_RenderUTF8_Blended(window->GetFont(font, language), " ", GetColour(colour)); 
    texture.GetTexture() = SDL_CreateTextureFromSurface(RenderWindow::renderer, surface);

    texture.SrcRect().w = surface->w;
    texture.SrcRect().h = surface->h;
    texture.DestRect().w = surface->w;
    texture.DestRect().h = surface->h;
}

TextButton::TextButton(Button::Type buttonType, std::string caption, int x, int y, Colour colour, FontType font, int bgOpacity, bool isSelected, Language language)
    : Button(buttonType, {0,0,0,0}, {x,y,0,0}, true, 5, isSelected, GetRadiusFromFont(font)), caption(caption), captionType(Lang::NONE),
        surface(nullptr), font(font), colour(colour), backgroundOpacity(bgOpacity)
    {

    surface = TTF_RenderUTF8_Blended(window->GetFont(font, language), caption.c_str(), GetColour(colour));    
    texture.GetTexture() = SDL_CreateTextureFromSurface(RenderWindow::renderer, surface);

    texture.SrcRect().w = surface->w;
    texture.SrcRect().h = surface->h;
    texture.DestRect().w = surface->w;
    texture.DestRect().h = surface->h;
}

ImageButton::ImageButton(Button::Type buttonType, SDL_Rect destRect, const char* path, bool isSelected)
    : Button(buttonType, {0,0,destRect.w,destRect.h}, destRect, false, 5, isSelected) {
    
    texture.LoadTexture(path);
}

void TextButton::DrawButton() {
    if (backgroundOpacity > 0)
        roundedBoxRGBA(RenderWindow::renderer, texture.GetDestX1() - radius, texture.GetDestY1() - radius, texture.GetDestX2() + radius, texture.GetDestY2() + radius, radius, 255, 255, 255, backgroundOpacity);
    
    texture.Render();

    if (isSelected)
        DrawSelectBox();
}

void ImageButton::DrawButton() {
    texture.Render();
    if (isSelected)
        DrawSelectBox();
}

void TextButton::DrawSelectBox() {
    // Texture already loaded in
    if (selectBox.GetTexture() != nullptr) {
        selectBox.Render();
        return;
    }

    /*** Loading Texture ***/
    SDL_DestroyTexture(selectBox.GetTexture());

    const int r=255, g=215, b=0, a=255;

    int margin = radius - padding;
    if (margin < 3) margin = 3;

    int frameWidth = texture.GetDestX2() - texture.GetDestX1() + radius * 2 +1;
    int frameHeight = texture.GetDestY2() - texture.GetDestY1() + radius * 2 +1;

    // Create a transparent target texture
    selectBox.GetTexture() = SDL_CreateTexture(RenderWindow::renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, frameWidth, frameHeight);
    SDL_SetTextureBlendMode(selectBox.GetTexture(), SDL_BLENDMODE_BLEND);

    // Set render target to our texture
    SDL_SetRenderTarget(RenderWindow::renderer, selectBox.GetTexture());
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
    selectBox.DestRect().x = texture.GetDestX1() - radius;
    selectBox.DestRect().y = texture.GetDestY1() - radius;
    selectBox.DestRect().w = frameWidth;
    selectBox.DestRect().h = frameHeight;

    selectBox.SrcRect().x = 0;
    selectBox.SrcRect().y = 0;
    selectBox.SrcRect().w = frameWidth;
    selectBox.SrcRect().h = frameHeight;

    // Render Texture
    selectBox.Render();
}

void ImageButton::DrawSelectBox() {
    // Texture already loaded in
    if (selectBox.GetTexture() != nullptr) {
        selectBox.Render();
        return;
    }

    /*** Loading Texture ***/
    SDL_DestroyTexture(selectBox.GetTexture());

    const int r=255, g=215, b=0, a=255;
    SDL_Renderer* renderer = RenderWindow::renderer;

    int frameWidth = texture.GetDestX2() - texture.GetDestX1() + padding * 2;
    int frameHeight = texture.GetDestY2() - texture.GetDestY1() + padding * 2;
    
    // Create a transparent target texture
    selectBox.GetTexture() = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, frameWidth, frameHeight);
    SDL_SetTextureBlendMode(selectBox.GetTexture(), SDL_BLENDMODE_BLEND);

    // Set render target to our texture
    SDL_SetRenderTarget(renderer, selectBox.GetTexture());
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
    selectBox.DestRect().x = texture.GetDestX1() - padding;
    selectBox.DestRect().y = texture.GetDestY1() - padding;
    selectBox.DestRect().w = frameWidth;
    selectBox.DestRect().h = frameHeight;

    selectBox.SrcRect().x = 0;
    selectBox.SrcRect().y = 0;
    selectBox.SrcRect().w = frameWidth;
    selectBox.SrcRect().h = frameHeight;

    // Render Texture
    selectBox.Render();
}

void TextButton::DestroySelectBoxTexture() { selectBox.DeleteTexture(); }

Lang::CaptionType TextButton::GetCaptionType() const { return captionType; }

void TextButton::UpdateCaption(std::string newCaption, Language newLanguage) {
    caption = newCaption;

    SDL_DestroyTexture(texture.GetTexture());
    SDL_FreeSurface(surface);
    surface = nullptr;

    surface = TTF_RenderUTF8_Blended(window->GetFont(font, newLanguage), caption.c_str(), GetColour(colour));    
    texture.GetTexture() = SDL_CreateTextureFromSurface(RenderWindow::renderer, surface);

    texture.SrcRect().w = surface->w;
    texture.SrcRect().h = surface->h;
    texture.DestRect().w = surface->w;
    texture.DestRect().h = surface->h;
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
    Button::window = this;

    LoadFonts();
}

void RenderWindow::Clear() { SDL_RenderClear(renderer); }

void RenderWindow::Display() { SDL_RenderPresent(renderer); }

void RenderWindow::RenderText(std::string text, int x, int y, Colour colour, FontType font, Language language) {
    SDL_Surface* captionSurface = nullptr;
    switch (language) {
    case ENGLISH:
    case HUNGARIAN:
        captionSurface = TTF_RenderUTF8_Blended(latinFonts.GetFont(font), text.c_str(), GetColour(colour));
        break;
    case JAPANESE:
        captionSurface = TTF_RenderUTF8_Blended(japaneseFonts.GetFont(font), text.c_str(), GetColour(colour));
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

void RenderWindow::ApplyTransition(int transparency) {
    boxRGBA(renderer, 0, 0, width-1, height-1, 0, 0, 0, transparency);
}

void RenderWindow::DrawBackground(int r, int g, int b) {
    boxRGBA(renderer, 0, 0, width-1, height-1, r, g, b, 255);
}

void RenderWindow::LoadFonts() {
    japaneseFonts.LoadFont("../res/font/NotoSansJP-Regular.ttf", 30, REG30);
    latinFonts.LoadFont("../res/font/OpenSans-Regular.ttf", 30, REG30);
    
    japaneseFonts.LoadFont("../res/font/NotoSansJP-Bold.ttf", 100, BOLD100);
    latinFonts.LoadFont("../res/font/OpenSans-Bold.ttf", 100, BOLD100);

    japaneseFonts.LoadFont("../res/font/NotoSansJP-Medium.ttf", 50, MED50);
    latinFonts.LoadFont("../res/font/OpenSans-Medium.ttf", 50, MED50);

    japaneseFonts.LoadFont("../res/font/NotoSansJP-Medium.ttf", 15, MED15);
    latinFonts.LoadFont("../res/font/OpenSans-Medium.ttf", 15, MED15);
}

int RenderWindow::GetWidth() const { return width; }

int RenderWindow::GetHeight() const { return height; }

SDL_Renderer* RenderWindow::GetRenderer() { return renderer; }

TTF_Font* RenderWindow::GetFont(FontType font, Language language) {
    switch (language){
    case ENGLISH:
    case HUNGARIAN:
        return latinFonts.GetFont(font);
    case JAPANESE:
        return japaneseFonts.GetFont(font);
    
    default:
        throw "Language not found! RenderWindow::GetFont()";
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
void Transition::SetTransition(size_t milliSeconds) {
    AlreadyReachedMiddle = false;
    #ifdef QUICK
        timer.Activate(200);
        return;
    #endif
    timer.Activate(milliSeconds);
}

void Transition::Deactivate() { timer.Deactivate(); }

int Transition::GetTransparency() const {
    if (timer.IsActive() == false)
        return 0;
    float percentage = timer.GetPercent();
    if (percentage > 1.0f)
        return 0;

    // Calculating Transparency
    if (percentage < 0.4f)
        return percentage * 637.5f; // 637,5 = 255 / 0,4

    if (percentage > 0.6f)
        return (1.0f - percentage) * 637.5f;

    return 255;
}

float Transition::GetPercent() const { return timer.GetPercent(); }

bool Transition::IsActive() const { return timer.IsActive(); }

bool Transition::HasExpired() const { return timer.HasExpired(); }

bool Transition::IsMiddle() const { return AlreadyReachedMiddle == false && timer.GetPercent() > 0.5f; }

void Transition::ReachMiddle() { AlreadyReachedMiddle = true; }

Transition::~Transition() {
    #ifdef DTOR
    std::clog << "~Transition Dtor" << std::endl;
    #endif
}
/* ************************************************************************************ */

/***** GLOBAL FUNCTIONS *****/

SDL_Color GetColour(Colour colour) {
    switch (colour) {
    case WHITE:
        return (SDL_Color) {255, 255, 255};
    case BLACK:
        return (SDL_Color) {0, 0, 0};
    
    default:
        throw "\nColour not found! GetColour()\n";
        break;
    }
}

int GetRadiusFromFont(FontType font) {
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