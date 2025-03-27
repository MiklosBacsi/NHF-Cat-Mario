#ifndef RENDER_WINDOW_H
#define RENDER_WINDOW_H

#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

class RenderWindow {
private:
    SDL_Window* window;
    SDL_Renderer* renderer;
public:
    RenderWindow(const char* title = "Cat Mario", int width=1600, int height=900);
};

#endif