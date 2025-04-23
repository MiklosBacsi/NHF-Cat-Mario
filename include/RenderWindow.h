#ifndef RENDER_WINDOW_H
#define RENDER_WINDOW_H

#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <vector>

#include "LanguageModule.h"
#include "Timer.h"

enum Colour { WHITE=0, BLACK };
enum FontType { REG30=0, BOLD100, MED50, MED15, FONT_COUNT=4 };

SDL_Color getColour(Colour colour);
int getRadiusFromFont(FontType font);

class Texture {
private:
    SDL_Texture* texture;
    SDL_Rect destRect;
public:
    Texture(SDL_Texture* texture = nullptr, SDL_Rect destRect={.x=0, .y=0, .w=0, .h=0});
    SDL_Texture*& getTexture();
    const SDL_Rect* getDestRect() const;
    int getWidth() const;
    int getHeight() const;
    int getX1() const;
    int getX2() const;
    int getY1() const;
    int getY2() const;
    void setX(int x);
    void setY(int y);
    void setWidth(int width);
    void setHeight(int height);
    ~Texture();
};

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
    SDL_Renderer* renderer;
    int width, height;
    Font latinFonts;
    Font japaneseFonts;

    void loadFonts();
public:
    RenderWindow(const char* title, int width, int height);
    Texture loadTexture(const char* path, int width, int height);
    void loadTexture(const char* path, Texture& texture);
    void clear();
    void render(Texture& texture, int destX=0, int destY=0);
    void render(Texture& texture, const SDL_Rect* srcRect, const SDL_Rect* destRect);
    void display();
    int getWidth() const;
    int getHeight() const;
    SDL_Renderer* getRenderer();
    TTF_Font* getFont(FontType font, Language language);
    void renderText(std::string text, int x, int y, Colour colour, FontType font, Language language);
    void applyTransition(int transparency);
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
    Button(Type type, SDL_Rect destRect, RenderWindow& window, bool isTextBased, int padding, bool isSelected=false);
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
    ImageButton(Button::Type buttonType, SDL_Rect destRect, const char* path, RenderWindow& window, bool isSelected=false);
    void drawButton(RenderWindow& window);
    ~ImageButton();
};

class Transition {
private:
    Timer timer;
public:
    Transition() {}
    void setTransition(size_t miliSeconds);
    int getTransparency();
    bool getIsActive() const;
};

#endif // RENDER_WINDOW_H