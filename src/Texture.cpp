#ifndef CPORTA

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <iostream>
#include <string>

#include "Texture.h"

SDL_Renderer* Texture::renderer = nullptr;

SDL_Texture* Texture::LoadStaticTexture(std::string path) {
    SDL_Texture* staticTexture = IMG_LoadTexture(Texture::renderer, path.c_str());
    
    if (staticTexture == nullptr)
        std::cerr << "Failed to load static texture. Error: " << SDL_GetError() << std::endl;

    return staticTexture;
}

Texture::Texture(bool isStatic) : texture(nullptr), srcRect({0,0,0,0}), destRect({0,0,0,0}), isStatic(isStatic) {}

Texture::Texture(std::string path, SDL_Rect rect, bool isStatic)
    : texture(nullptr), srcRect(rect), destRect(rect), isStatic(isStatic) {

    LoadTexture(path.c_str());
}

Texture::Texture(std::string path, SDL_Rect srcRect, SDL_Rect destRect, bool isStatic)
    : texture(nullptr), srcRect(srcRect), destRect(destRect), isStatic(isStatic) {

    LoadTexture(path.c_str());
}

Texture::Texture(SDL_Texture* texture, SDL_Rect srcRect, SDL_Rect destRect, bool isStatic)
    : texture(texture), srcRect(srcRect), destRect(destRect), isStatic(isStatic) {
}

void Texture::Render() {
    SDL_RenderCopy(Texture::renderer, texture, &srcRect, &destRect);
}

void Texture::LoadTexture(std::string path) {
    if (isStatic && texture != nullptr)
        return;
    
    if (isStatic == false)
        DeleteTexture();

    texture = IMG_LoadTexture(Texture::renderer, path.c_str());
    if (texture == nullptr)
        std::cerr << "Failed to load texture. Error: " << SDL_GetError() << std::endl;
}

void Texture::DeleteTexture() {
    if (texture != nullptr) {
        SDL_DestroyTexture(texture);
        texture = nullptr;
    }
}

int Texture::GetDestX1() const { return destRect.x; }

int Texture::GetDestX2() const { return destRect.x + destRect.w; }

int Texture::GetDestY1() const { return destRect.y; }

int Texture::GetDestY2() const { return destRect.y + destRect.h; }

SDL_Texture*& Texture::GetTexture() { return texture; }

SDL_Rect& Texture::SrcRect() { return srcRect; }

SDL_Rect& Texture::DestRect() { return destRect; }

Texture::~Texture() {
    if (isStatic == false)
        DeleteTexture();

    #ifdef DTOR
    std::clog << "~Texture Dtor" << std::endl;
    #endif
}



#endif // CPORTA