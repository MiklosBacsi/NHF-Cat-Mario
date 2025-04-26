#ifndef RENDER_WINDOW_H
#define RENDER_WINDOW_H

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

#include "Texture.h"
#include "LanguageModule.h"
#include "Timer.h"

enum Colour { WHITE=0, BLACK };
enum FontType { REG30=0, BOLD100, MED50, MED15, FONT_COUNT=4 };

SDL_Color getColour(Colour colour);
int getRadiusFromFont(FontType font);

class Font {
private:
    std::vector<TTF_Font*> fonts;
public:
    Font();
    void loadFont(const char* path, int size, FontType fontType);
    TTF_Font* getFont(FontType font);
    ~Font();
};

class RenderWindow {
private:
    SDL_Window* window;
    int width, height;
    Font latinFonts;
    Font japaneseFonts;

    void loadFonts();
public:
    RenderWindow(const char* title, int width, int height);
    void clear();
    void display();
    void renderText(std::string text, int x, int y, Colour colour, FontType font, Language language);
    void drawBackground(int r=166, int g=181, int b=246);
    void applyTransition(int transparency);
    int getWidth() const;
    int getHeight() const;
    SDL_Renderer* getRenderer();
    TTF_Font* getFont(FontType font, Language language);

    static SDL_Renderer* renderer;
    ~RenderWindow();
};

class Button {
public:
    enum Type { NONE=0, START, EXIT, PAUSE, CONTINUE, ENG, JP, HUN, LEV1, LEV2 };
protected:
    Type buttonType;
    Texture texture;
    Texture selectBox;
    bool isSelected;
    bool isTextBased;
    const int padding;
public:
    Button(Type type, SDL_Rect srcRect, SDL_Rect destRect, bool isTextBased, int padding, bool isSelected=false);
    virtual void drawButton(RenderWindow& window) = 0;
    bool isClicked(int x, int y) const;
    bool getIsSelected() const;
    bool getIsTextBased() const;
    Type getButtonType() const;
    void setSelected(bool selected);
    virtual ~Button();
};

class TextButton : public Button {
private:
    std::string caption;
    Lang::CaptionType captionType;
    SDL_Surface* surface;
    FontType font;
    Colour colour;
    const int backgroundOppacity;
    const int radius;

    void drawSelectBox(RenderWindow& window);
public:
    TextButton(Button::Type buttonType, Lang::CaptionType capType, int x, int y, Colour colour, FontType font, Language language, RenderWindow& window, int bgOpacity=0, bool isSelected=false);
    TextButton(Button::Type buttonType, std::string capiton, int x, int y, Colour colour, FontType font, RenderWindow& window, int bgOpacity=0, bool isSelected=false);
    void drawButton(RenderWindow& window);
    void updateCaption(std::string newCaption, Language newLanguage, RenderWindow& window); // when changing Language
    void destroySelectBoxTexture();
    Lang::CaptionType getCaptionType() const;
    ~TextButton();
};

class ImageButton : public Button {
private:
    void drawSelectBox(RenderWindow& window);
public:
    ImageButton(Button::Type buttonType, SDL_Rect destRect, const char* path, bool isSelected=false);
    void drawButton(RenderWindow& window);
    ~ImageButton();
};

class Transition {
private:
    Timer timer;
    bool AlreadyReachedMiddle;
public:
    Transition() {}
    void setTransition(size_t miliSeconds);
    void deactivate();
    int getTransparency() const;
    float getPercent() const;
    bool getIsActive() const;
    bool hasExpired() const;
    bool isMiddle() const;
    void reachMiddle();
    ~Transition();
};

#endif // CPORTA

#endif // RENDER_WINDOW_H