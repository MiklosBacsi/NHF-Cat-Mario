#ifndef RENDER_WINDOW_H
#define RENDER_WINDOW_H

#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <vector>

#include "LanguageModule.h"

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

enum Colour {WHITE=0, BLACK};
enum FontType {REG30=0, MED20, FONT_COUNT=2};

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

    SDL_Color getColour(Colour colour);
    TTF_Font* getFont(FontType font, Language language);
    void loadFonts();
public:
    RenderWindow(const char* title, int width, int height);
    Texture loadTexture(const char* file, int width, int height);
    void clear();
    void render(Texture& texture, int destX=0, int destY=0);
    void display();
    int getWidth() const;
    int getHeight() const;
    void renderText(std::string text, int x, int y, Colour colour, FontType font, Language language);
    ~RenderWindow();
};

#endif // RENDER_WINDOW_H