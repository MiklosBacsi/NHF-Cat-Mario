#ifndef COIN_ANIMATION_H
#define COIN_ANIMATION_H

#ifndef CPORTA

#include <SDL2/SDL.h>

#include <iostream>
#include <vector>

#include "Texture.h"
#include "Sound.h"

/**
 * @brief Single coin used for animation.
 */
struct Coin {
    SDL_Rect destRect;  ///< Destination rectangle for the Texture.
    float lifeTime;     ///< Remaining life time.
    int x;      ///< Relative x coordinate.
    float y;    ///< y coordinate.
};


/**
 * @brief Single coin used for animation.
 */
class CoinAnimation {
public:
    enum Type { COIN=0, BRICK };    ///< Type of animation.

private:
    SDL_Rect srcRect;           ///< Source rectangle for the Texture.
    SDL_Texture* texture;       ///< Destination rectangle for the Texture.
    std::vector<Coin> coins;    ///< Contains coins.
    float frameDelay;           ///< Necessary for proper movement of animations.

public:
    /**
     * Sets up the animation container and other members.
     * @param path Path to Coin texture.
     * @param frameDelay Necessary for proper movement of animations.
     * @see Coin
     */
    CoinAnimation(std::string path, float frameDelay);

    /**
     * @brief Adds a new coin to the container.
     * @param x Relative x coordinate.
     * @param y y coordinate.
     * @see Coin
     */
    void AddCoin(int x, int y);

    /**
     * @brief Renders existing animations to the screen (renderer), and removes the expired ones (life time below zero).
     * @param cameraX How much the camera is shifted (to the right).
     * @param renderer The animations are rendered onto this.
     * @see SDL_Renderer
     */
    void Render(int cameraX, SDL_Renderer* renderer);

    /**
     * Destructor.
     */
    ~CoinAnimation();
};




#endif // CPORTA

#endif // COIN_ANIMATION_H