#ifndef TEXTURE_H
#define TEXTURE_H

#ifndef CPORTA

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <iostream>
#include <string>

class Texture {
public:
    static SDL_Renderer* renderer;
private:
    SDL_Texture* texture;
    SDL_Rect srcRect;
    SDL_Rect destRect;
    bool isStatic;  // If static, it's the responsibility of the destructor to call deleteTextrure()
public:
    Texture(bool isStatic = false);
    Texture(std::string path, SDL_Rect rect, bool isStatic = false);
    Texture(std::string path, SDL_Rect srcRect, SDL_Rect destRect, bool isStatic = false);
    Texture(SDL_Texture* texture, SDL_Rect srcRect, SDL_Rect destRect, bool isStatic = false);
    void render();
    void loadTexture(const char* path);
    void deleteTexture();
    SDL_Texture*& getTexture();
    SDL_Rect& getSrcRect();
    SDL_Rect& getDestRect();
    int getDestX1() const;
    int getDestX2() const;
    int getDestY1() const;
    int getDestY2() const;
    ~Texture();
};

#endif // CPORTA

#endif // TEXTURE_H