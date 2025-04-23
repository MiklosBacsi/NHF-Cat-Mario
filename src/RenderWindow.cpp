#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include <iostream>

#include "RenderWindow.h"
#include "LanguageModule.h"

using std::cout; using std::endl;


/***** CLASS TEXTURE *****/
Texture::Texture(SDL_Texture* texture, SDL_Rect destRect) : texture(texture), destRect(destRect) {}
SDL_Texture*& Texture::getTexture() { return texture; }
const SDL_Rect* Texture::getDestRect() const { return &destRect; }
int Texture::getWidth() const { return destRect.w; }
int Texture::getHeight() const { return destRect.h; }
int Texture::getX1() const { return destRect.x; }
int Texture::getX2() const { return destRect.x + destRect.w; }
int Texture::getY1() const { return destRect.y; }
int Texture::getY2() const { return destRect.y + destRect.h; }
void Texture::setX(int x) { destRect.x = x; }
void Texture::setY(int y) { destRect.y = y; }
void Texture::setWidth(int width) { destRect.w = width; }
void Texture::setHeight(int height) { destRect.h = height; }

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
Button::Button(Button::Type buttonType, SDL_Rect destRect, RenderWindow& window, bool isTextBased, int padding, bool isSelected)
    : buttonType(buttonType), texture(nullptr, destRect), isSelected(isSelected), isTextBased(isTextBased), padding(padding) {
}

bool Button::isClicked(int x, int y) const {
    if (x >= texture.getX1() && x <= texture.getX2() && y >= texture.getY1() && y <= texture.getY2())
        return true;
    return false;
}

bool Button::getIsSelected() const { return isSelected; }

bool Button::getIsTextBased() const { return isTextBased; }

Button::Type Button::getButtonType() const { return buttonType; }

void Button::setSelected(bool selected) {
    isSelected = selected;
    
    // Destroying old selectBox texture
    if (selected == false) {
        SDL_DestroyTexture(selectBox.getTexture());
        selectBox.getTexture() = nullptr;
    }
}

Button::~Button() {
    cout << "~Button Dtor" << endl;
}

TextButton::TextButton(Button::Type buttonType, Lang::CaptionType capType, int x, int y, Colour colour, FontType font, Language language, RenderWindow& window, int bgOpacity, bool isSelected)
    : Button(buttonType, {x, y, 0, 0}, window, true, 5, isSelected), caption(" "), captionType(capType), surface(nullptr), font(font), colour(colour), backgroundOppacity(bgOpacity), radius(getRadiusFromFont(font)) {

    surface = TTF_RenderUTF8_Blended(window.getFont(font, language), " ", getColour(colour));    
    texture.getTexture() = SDL_CreateTextureFromSurface(window.getRenderer(), surface);

    texture.setWidth(surface->w);
    texture.setHeight(surface->h);
}

ImageButton::ImageButton(Button::Type buttonType, SDL_Rect destRect, const char* path, RenderWindow& window, bool isSelected)
    : Button(buttonType, destRect, window, false, 5, isSelected) {
    
    window.loadTexture(path, texture);
}

void TextButton::drawButton(RenderWindow& window) {
    if (backgroundOppacity > 0)
        roundedBoxRGBA(window.getRenderer(), texture.getX1() - radius, texture.getY1() - radius, texture.getX2() + radius, texture.getY2() + radius, radius, 255, 255, 255, backgroundOppacity);
    
    window.render(texture, nullptr, texture.getDestRect());

    if (isSelected)
        drawSelectBox(window);
}

void ImageButton::drawButton(RenderWindow& window) {
    window.render(texture, nullptr, texture.getDestRect());
    if (isSelected)
        drawSelectBox(window);
}

void TextButton::drawSelectBox(RenderWindow& window) {
    // Texture already loaded in
    if (selectBox.getTexture() != nullptr) {
        window.render(selectBox, nullptr, selectBox.getDestRect());
        return;
    }

    /*** Loading Texture ***/
    SDL_DestroyTexture(selectBox.getTexture());

    const int r=255, g=215, b=0, a=255;
    SDL_Renderer* renderer = window.getRenderer();

    int frameWidth = texture.getX2() - texture.getX1() + padding * 2;
    int frameHeight = texture.getY2() - texture.getY1() + padding * 2;

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
    selectBox.setX(texture.getX1() - padding);
    selectBox.setY(texture.getY1() - padding);
    selectBox.setWidth(frameWidth);
    selectBox.setHeight(frameHeight);

    // Render Texture
    window.render(selectBox, nullptr, selectBox.getDestRect());
}

void ImageButton::drawSelectBox(RenderWindow& window) {
    // Texture already loaded in
    if (selectBox.getTexture() != nullptr) {
        window.render(selectBox, nullptr, selectBox.getDestRect());
        return;
    }

    /*** Loading Texture ***/
    SDL_DestroyTexture(selectBox.getTexture());

    const int r=255, g=215, b=0, a=255;
    SDL_Renderer* renderer = window.getRenderer();

    int frameWidth = texture.getX2() - texture.getX1() + padding * 2;
    int frameHeight = texture.getY2() - texture.getY1() + padding * 2;
    
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
    selectBox.setX(texture.getX1() - padding);
    selectBox.setY(texture.getY1() - padding);
    selectBox.setWidth(frameWidth);
    selectBox.setHeight(frameHeight);

    // Render Texture
    window.render(selectBox, nullptr, selectBox.getDestRect());
}

void TextButton::destroySelectBoxTexture() {
    SDL_DestroyTexture(selectBox.getTexture());
    selectBox.getTexture() = nullptr;
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

    Texture newTexture(texture, { 0, 0, width, height });
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
    boxRGBA(renderer, 0, 0, width-1, height-1, 0, 0, 0, transparency);
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