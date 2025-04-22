#ifndef RENDER_WINDOW_H
#define RENDER_WINDOW_H

#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <vector>

#include "LanguageModule.h"

enum Colour { WHITE=0, BLACK };
enum FontType { REG30=0, MED20, FONT_COUNT=2 };
enum buttonType { NONE=0, START, EXIT, ENG, JP, HUN, LEV1, LEV2 };

SDL_Color getColour(Colour colour);

class Font {
private:
    std::vector<TTF_Font*> fonts;
public:
    Font();
    void loadFont(const char* path, int size, FontType fontType);
    TTF_Font* getFont(FontType font);
    ~Font();
};

class Texture;

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
    Texture loadTexture(const char* file, int width, int height);
    void clear();
    void render(Texture& texture, int destX=0, int destY=0);
    void render(Texture& texture, const SDL_Rect* srcRect, const SDL_Rect* destRect);
    void display();
    int getWidth() const;
    int getHeight() const;
    SDL_Renderer* getRenderer();
    TTF_Font* getFont(FontType font, Language language);
    void renderText(std::string text, int x, int y, Colour colour, FontType font, Language language);
    ~RenderWindow();
};

class Texture {
private:
    SDL_Texture* texture;
    int width;
    int height;
public:
    Texture(SDL_Texture* texture = nullptr, int width=0, int height=0)
    : texture(texture), width(width), height(height) {}
    SDL_Texture* getTexture();
    int getWidth() const;
    int getHeight() const;
    ~Texture();
};

class Button {
protected:
    SDL_Rect* srcRect;
    SDL_Texture* texture;
    bool isSelected;
    bool isTextBased;
    const int padding;

    void drawSelectBox(SDL_Renderer* renderer);
public:
    Button(int x, int y, RenderWindow& window, bool isTextBased, int padding, bool isSelected=false);
    virtual void drawButton(SDL_Renderer* renderer) = 0;
    bool isClicked(int x, int y) const;
    bool getSelected() const;
    void setSelected(bool isSelected);
    virtual ~Button();
};

class TextButton : public Button {
private:
    std::string caption;
    SDL_Surface* surface;
    FontType font;
    Colour colour;
    const int backgroundOppacity;
public:
    TextButton(std::string text, int x, int y, Colour colour, FontType font, Language language, RenderWindow& window, int bgOpacity=0, bool isSelected=false);
    void drawButton(SDL_Renderer* renderer);
    void updateCaption(Language newLanguage, RenderWindow& window); // when changing Language
    ~TextButton();
};

class ImageButton : public Button {
public:
    ImageButton(int x, int y, const char* path, int width, int height, RenderWindow& window, bool isSelected=false);
    void drawButton(SDL_Renderer* renderer);
    ~ImageButton();
};

#endif // RENDER_WINDOW_H