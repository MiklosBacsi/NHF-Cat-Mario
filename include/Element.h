#ifndef ELEMENT_H
#define ELEMENT_H

#ifndef CPORTA

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <iostream>

#include "Texture.h"

class Element {
public:
    enum Type { CLOUD, TUBE, MOUNTAIN }; //etc
    static SDL_Texture* textures;
private:
    Texture texture;
public:
    Element();
    void update(float dt);
    void render(float cameraX);
    void activate();
    ~Element(); // deletes the static SDL_Texture* with textures.deleteTexture()
};












#endif // CPORTA

#endif // ELEMENT_H