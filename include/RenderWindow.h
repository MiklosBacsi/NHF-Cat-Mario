#ifndef RENDER_WINDOW_H
#define RENDER_WINDOW_H

#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

class Texture {
private:
    SDL_Texture* texture;
    int width;
    int height;
public:
    Texture(SDL_Texture* texture = nullptr, int width=0, int height=0)
    : texture(texture), width(width), height(height) {}
    SDL_Texture* getTexture(); // SDL miatt nem lehet const!!!
    int getWidth() const;
    int getHeight() const;
    ~Texture();
};

class RenderWindow {
private:
    SDL_Window* window;
    SDL_Renderer* renderer;
public:
    RenderWindow(const char* title, int width, int height);
    Texture loadTexture(const char* file, int width, int height);
    void clear();
    void render(Texture& texture, int destX=0, int destY=0); // SDL miatt nem lehet const Texture& texture
    void display();
};

#endif