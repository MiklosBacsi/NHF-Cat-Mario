#ifndef CPORTA

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <iostream>
#include <string>

#include "Texture.h"

SDL_Renderer* Texture::renderer = nullptr;

Texture::Texture(bool isStatic) : texture(nullptr), srcRect({0,0,0,0}), destRect({0,0,0,0}), isStatic(isStatic) {}

Texture::Texture(std::string path, SDL_Rect rect, bool isStatic)
    : texture(nullptr), srcRect(rect), destRect(rect), isStatic(isStatic) {

    loadTexture(path.c_str());
}

Texture::Texture(std::string path, SDL_Rect srcRect, SDL_Rect destRect, bool isStatic)
    : texture(nullptr), srcRect(srcRect), destRect(destRect), isStatic(isStatic) {

    loadTexture(path.c_str());
}

Texture::Texture(SDL_Texture* texture, SDL_Rect srcRect, SDL_Rect destRect, bool isStatic)
    : texture(texture), srcRect(srcRect), destRect(destRect), isStatic(isStatic) {
}

void Texture::render() {
    SDL_RenderCopy(Texture::renderer, texture, &srcRect, &destRect);
}

void Texture::loadTexture(const char* path) {
    if (isStatic && texture != nullptr)
        return;
    
    if (isStatic == false)
        deleteTexture();

    texture = IMG_LoadTexture(Texture::renderer, path);
    if (texture == nullptr)
        std::cerr << "Failed to load texture. Error: " << SDL_GetError() << std::endl;
}

void Texture::deleteTexture() {
    if (texture != nullptr) {
        SDL_DestroyTexture(texture);
        texture = nullptr;
    }
}

int Texture::getDestX1() const { return destRect.x; }

int Texture::getDestX2() const { return destRect.x + destRect.w; }

int Texture::getDestY1() const { return destRect.y; }

int Texture::getDestY2() const { return destRect.y + destRect.h; }

SDL_Texture*& Texture::getTexture() { return texture; }

SDL_Rect& Texture::getSrcRect() { return srcRect; }

SDL_Rect& Texture::getDestRect() { return destRect; }

Texture::~Texture() {
    if (isStatic == false)
        deleteTexture();

    #ifdef DTOR
    std::clog << "~Texture Dtor" << std::endl;
    #endif
}



#endif // CPORTA