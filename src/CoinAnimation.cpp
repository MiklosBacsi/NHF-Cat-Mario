#ifndef CPORTA

#include <SDL2/SDL.h>

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

#include "CoinAnimation.h"
#include "Texture.h"

CoinAnimation::CoinAnimation(std::string path, float frameDelay)
    : srcRect({0, 0, 20, 28}), texture(nullptr), frameDelay(frameDelay) {
    
    texture = Texture::LoadStaticTexture(path.c_str());
}

void CoinAnimation::AddCoin(int x, int y) {
    coins.push_back((Coin) {
        .destRect = {x, y, 50, 70},
        .lifeTime = 0.5f,
        .x = x - 25,
        .y = (float) y - 50
    });
}

void CoinAnimation::Render(int cameraX, SDL_Renderer* renderer) {
    for (Coin& coin : coins) {
        coin.lifeTime -= frameDelay;
        coin.y -= frameDelay * 500.0f;
        coin.destRect.x = coin.x - cameraX;
        coin.destRect.y = (int) coin.y;
        
        SDL_RenderCopy(renderer, texture, &srcRect, &(coin.destRect));
    }

    coins.erase(std::remove_if(coins.begin(), coins.end(),
        [](const Coin& coin) { return coin.lifeTime < 0.0f; }), coins.end());
}

CoinAnimation::~CoinAnimation() {
    if (texture != nullptr)
        SDL_DestroyTexture(texture);
    #ifdef DTOR
    std::clog << "~CoinAnimation Dtor" << std::endl;
    #endif
}

#endif // CPORTA