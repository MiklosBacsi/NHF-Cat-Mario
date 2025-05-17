#ifndef TEXTURE_H
#define TEXTURE_H

#ifndef CPORTA

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <iostream>
#include <string>


/** 
 * @brief Class Texture containing texture, srcRect and destRect.
 */
class Texture {
public:
    static SDL_Renderer* renderer; ///< Static member, necessary for rendering the textures.
    /** 
     * @brief Loads a static texture from path.
     * @param path Path to the texture.
     * @see SDL_Texture
     * @return Return loaded texture.
     */
    static SDL_Texture* LoadStaticTexture(std::string path);

private:
    SDL_Texture* texture;   ///< Pointer to texture.
    SDL_Rect srcRect;       ///< Source rectangle for the Texture.
    SDL_Rect destRect;      ///< Destination rectangle for the Texture.
    bool isStatic;  ///< If static, it's the responsibility of the destructor to call deleteTexture().

public:
    /** 
     * @brief Constructor with no texture.
     * @param isStatic Contains if texture is static.
     */
    Texture(bool isStatic = false);

    /** 
     * @brief Constructor.
     * @param path Path to the texture.
     * @param rect Rectangles (source, destination).
     * @param isStatic Contains whether texture is static.
     * @see SDL_Rect
     */
    Texture(std::string path, SDL_Rect rect, bool isStatic = false);

    /** 
     * @brief Constructor.
     * @param path Path to the texture.
     * @param srcRect Source rectangle for the Texture.
     * @param destRect Destination rectangle for the Texture.
     * @param isStatic Contains whether texture is static.
     * @see SDL_Rect
     */
    Texture(std::string path, SDL_Rect srcRect, SDL_Rect destRect, bool isStatic = false);

    /** 
     * @brief Constructor.
     * @param texture Contains texture.
     * @param srcRect Source rectangle for the Texture.
     * @param destRect Destination rectangle for the Texture.
     * @param isStatic Contains whether texture is static.
     * @see SDL_Rect
     */
    Texture(SDL_Texture* texture, SDL_Rect srcRect, SDL_Rect destRect, bool isStatic = false);

    /**
     * @brief Renders the texture to the screen (renderer).
     */
    void Render();

    /**
     * @brief Loads a texture from file path.
     * @param path Path to texture.
     */
    void LoadTexture(std::string path);

    /**
     * @brief Deletes texture.
     */
    void DeleteTexture();

    /**
     * @return Returns pointer reference to texture.
     */
    SDL_Texture*& GetTexture();

    /**
     * @return Returns reference to source rectangle of Texture.
     */
    SDL_Rect& SrcRect();

    /**
     * @return Returns reference to destination rectangle of Texture.
     */
    SDL_Rect& DestRect();

    /**
     * @return Returns x1 coordinate of destination rectangle.
     */
    int GetDestX1() const;

    /**
     * @return Returns x2 coordinate of destination rectangle.
     */
    int GetDestX2() const;

    /**
     * @return Returns y1 coordinate of destination rectangle.
     */
    int GetDestY1() const;

    /**
     * @return Returns y2 coordinate of destination rectangle.
     */
    int GetDestY2() const;

    /**
     * @brief Destructor.
     */
    ~Texture();
};

#endif // CPORTA

#endif // TEXTURE_H