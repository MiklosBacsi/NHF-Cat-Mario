#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include <iostream>

#include "RenderWindow.h"
#include "LanguageModule.h"

using std::cout; using std::endl;


/***** CLASS TEXTURE *****/
Texture::Texture(SDL_Texture* texture, SDL_Rect srcRect) : texture(texture), srcRect(srcRect) {}
Texture::Texture(SDL_Texture* texture, int width, int height)
    : texture(texture), srcRect((SDL_Rect){.x=0, .y=0, .w=width, .h=height}) {}
SDL_Texture*& Texture::getTexture() { return texture; }
const SDL_Rect* Texture::getSrcRect() const { return &srcRect; }
int Texture::getWidth() const { return srcRect.w; }
int Texture::getHeight() const { return srcRect.h; }
int Texture::getX1() const { return srcRect.x; }
int Texture::getX2() const { return srcRect.x + srcRect.w; }
int Texture::getY1() const { return srcRect.y; }
int Texture::getY2() const { return srcRect.y + srcRect.h; }
void Texture::setWidth(int width) { srcRect.w = width; }
void Texture::setHeight(int height) { srcRect.h = height; }

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
Button::Button(Button::Type buttonType, SDL_Rect srcRect, RenderWindow& window, bool isTextBased, int padding, bool isSelected)
    : buttonType(buttonType), texture(nullptr, srcRect), isSelected(isSelected), isTextBased(isTextBased), padding(padding) {
}

void Button::drawSelectBox(SDL_Renderer* renderer) {
    const int width = 4;
    const int r=255, g=215, b=0, a=255;
    // TOP
    thickLineRGBA(renderer, texture.getX1() - padding, texture.getY1() - padding, texture.getX2() + padding, texture.getY1() - padding, width, r, g, b, a);
    // BOTTOM
    thickLineRGBA(renderer, texture.getX1() - padding, texture.getY2() + padding, texture.getX2() + padding, texture.getY2() + padding, width, r, g, b, a);
    // LEFT
    thickLineRGBA(renderer, texture.getX1() - padding, texture.getY1() - padding, texture.getX1() - padding, texture.getY2() + padding, width, r, g, b, a);
    // RIGHT
    thickLineRGBA(renderer, texture.getX2() + padding, texture.getY1() - padding, texture.getX2() + padding, texture.getY2() + padding, width, r, g, b, a);
}

bool Button::isClicked(int x, int y) const {
    if (x >= texture.getX1() && x <= texture.getX2() && y >= texture.getY1() && y <= texture.getY2())
        return true;
    return false;
}

bool Button::getIsSelected() const { return isSelected; }

bool Button::getIsTextBased() const { return isTextBased; }

Button::Type Button::getButtonType() const { return buttonType; }

void Button::setSelected(bool selected) { isSelected = selected; }

Button::~Button() {
    cout << "~Button Dtor" << endl;
}

TextButton::TextButton(Button::Type buttonType, Lang::CaptionType capType, int x, int y, Colour colour, FontType font, Language language, RenderWindow& window, int bgOpacity, bool isSelected)
    : Button(buttonType, {x, y, 0, 0}, window, true, 5, isSelected), caption(" "), captionType(capType), surface(nullptr), font(font), colour(colour), backgroundOppacity(bgOpacity) {

    surface = TTF_RenderUTF8_Blended(window.getFont(font, language), " ", getColour(colour));    
    texture.getTexture() = SDL_CreateTextureFromSurface(window.getRenderer(), surface);

    texture.setWidth(surface->w);
    texture.setHeight(surface->h);
}

ImageButton::ImageButton(Button::Type buttonType, SDL_Rect srcRect, const char* path, RenderWindow& window, bool isSelected)
    : Button(buttonType, srcRect, window, false, 2, isSelected) {
    
    window.loadTexture(path, texture);
}

void TextButton::drawButton(RenderWindow& window) {
    if (backgroundOppacity > 0)
        boxRGBA(window.getRenderer(), texture.getX1() - padding, texture.getY1() - padding, texture.getX2() + padding, texture.getY2() + padding, 255, 255, 255, backgroundOppacity);
    
    window.render(texture, texture.getX1(), texture.getY1());

    if (isSelected)
        drawSelectBox(window.getRenderer());
}

void ImageButton::drawButton(RenderWindow& window) {
    window.render(texture, texture.getX1(), texture.getY1());
    if (isSelected)
        drawSelectBox(window.getRenderer());
}

Lang::CaptionType TextButton::getCaptionType() const { return captionType; }

void TextButton::updateCaption(std::string newCaption, Language newLanguage, RenderWindow& window) {
    caption = newCaption;

    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture.getTexture());

    surface = TTF_RenderUTF8_Blended(window.getFont(font, newLanguage), caption.c_str(), getColour(colour));    
    texture.getTexture() = SDL_CreateTextureFromSurface(window.getRenderer(), surface);

    texture.setWidth(surface->w);
    texture.setHeight(surface->h);
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

Texture RenderWindow::loadTexture(const char* path, int width, int height) {
    SDL_Texture* texture = nullptr;
    texture = IMG_LoadTexture(renderer, path);
    
    if (texture == nullptr)
        cout << "Failed to load texture. Error: " << SDL_GetError() << endl;

    Texture newTexture(texture, width, height);
    return newTexture;
}

void RenderWindow::loadTexture(const char* path, Texture& texture) {
    texture.getTexture() = IMG_LoadTexture(renderer, path);
    
    if (texture.getTexture() == nullptr)
        cout << "Failed to load texture. Error: " << SDL_GetError() << endl;
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

void RenderWindow::applyTransition(int transparency) {
    boxRGBA(renderer, 0, 0, width, height, 0, 0, 0, transparency);
}

void RenderWindow::loadFonts() {
    japaneseFonts.loadFont("../res/font/NotoSansJP-Regular.ttf", 30, REG30);
    latinFonts.loadFont("../res/font/OpenSans-Regular.ttf", 30, REG30);
    
    japaneseFonts.loadFont("../res/font/NotoSansJP-Bold.ttf", 100, BOLD100);
    latinFonts.loadFont("../res/font/OpenSans-Bold.ttf", 100, BOLD100);

    japaneseFonts.loadFont("../res/font/NotoSansJP-Medium.ttf", 50, MED50);
    latinFonts.loadFont("../res/font/OpenSans-Medium.ttf", 50, MED50);

    japaneseFonts.loadFont("../res/font/NotoSansJP-Light.ttf", 15, LIGHT15);
    latinFonts.loadFont("../res/font/OpenSans-Light.ttf", 15, LIGHT15);
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

/***** CLASS TRANSITION *****/
void Transition::setTransition(size_t miliSeconds) {
    timer.activate(miliSeconds);
}

int Transition::getTransparency() {
    if (timer.getIsActive() == false)
        return 0;
    float percentage = timer.getPercent();
    if (percentage > 1.0f){
        timer.deactivate();
        return 0;
    }
    // Calculating Transparency
    if (percentage < 0.4f)
        return percentage * 637.5f; // 637,5 = 255 / 0,4

    if (percentage > 0.6f)
        return (1.0f - percentage) * 637.5f;

    return 255;
}

bool Transition::getIsActive() const { return timer.getIsActive(); }
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