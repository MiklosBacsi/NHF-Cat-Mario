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
    void Render();
    void LoadTexture(const char* path);
    void DeleteTexture();
    SDL_Texture*& GetTexture();
    SDL_Rect& SrcRect();
    SDL_Rect& DestRect();
    int GetDestX1() const;
    int GetDestX2() const;
    int GetDestY1() const;
    int GetDestY2() const;
    ~Texture();
};

#endif // CPORTA

#endif // TEXTURE_H