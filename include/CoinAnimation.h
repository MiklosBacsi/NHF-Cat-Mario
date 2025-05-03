#ifndef COIN_ANIMATION_H
#define COIN_ANIMATION_H

#ifndef CPORTA

#include <SDL2/SDL.h>

#include <iostream>
#include <vector>

#include "Texture.h"
#include "Sound.h"

struct Coin {
    SDL_Rect destRect;
    float lifeTime;
    int x;
    float y;
};

class CoinAnimation {
public:
    enum Type { COIN=0, BRICK };
private:
    SDL_Rect srcRect;
    SDL_Texture* texture;
    std::vector<Coin> coins;
    float frameDelay;
public:
    CoinAnimation(std::string path, float frameDelay);
    void AddCoin(int x, int y);
    void Render(int cameraX, SDL_Renderer* renderer);
    ~CoinAnimation();
};




#endif // CPORTA

#endif // COIN_ANIMATION_H