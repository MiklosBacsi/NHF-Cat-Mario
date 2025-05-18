/** @file RenderWindow.h
  * @brief Contains declarations of class: Font, RenderWindow, Button (and its derived classes), Transition
  * 
  * By using the SDL2 library, these classes allow us to create the GUI of the program.
  * 
  * @author Bácsi Miklós
  * @date 2025-05-18
*/

#ifndef RENDER_WINDOW_H
#define RENDER_WINDOW_H

#ifndef CPORTA

#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include <iostream>
#include <string>
#include <vector>

#include "Texture.h"
#include "LanguageModule.h"
#include "Timer.h"

enum Colour { WHITE=0, BLACK }; ///< Types of colour.
enum FontType { REG30=0, BOLD100, MED50, MED15, FONT_COUNT=4 }; ///< Font types.

/**
 * @brief Global function: converts Colour to SDL_Color.
 * @param colour Colour to be converted.
 * @return Returns the colour converted to SDL_Color.
 */
SDL_Color GetColour(Colour colour);

/**
 * @brief Global function: returns radius of the given font.
 * @param font Its radius is returned ("calculated" from the size of the font).
 * @return Returns the radius of the given font.
 */
int GetRadiusFromFont(FontType font);

/****************************************************************************************************/

/**
 * @brief It contains all the fonts that are necessary for rendering text.
 */
class Font {
private:
    std::vector<TTF_Font*> fonts;   ///< It stores the loaded fonts.

public:
    /**
     * @brief Sets up the container and inits SDL's TTF library.
     */
    Font();

    /**
     * @brief Load a font with the given parameters.
     * @param path File path of the font.
     * @param fontType Type of the font.
     */
    void LoadFont(const char* path, int size, FontType fontType);

    /**
     * @param font Type of the font.
     * @return Returns a pointer to the asked font.
     */
    TTF_Font* GetFont(FontType font);

    /**
     * @brief Destructor: closes the open fonts.
     */
    ~Font();
};

/****************************************************************************************************/

/**
 * @brief Window of the program.
 */
class RenderWindow {
public:
    static SDL_Renderer* renderer;  ///< Static member, renderer of the program.
private:
    SDL_Window* window; ///< Pointer to the window.
    int width;  ///< Width of the window.
    int height; ///< Height of the window.
    Font latinFonts;    ///< Contains the latin fonts (for English and Hungarian).
    Font japaneseFonts; ///< Contains the japanese fonts.

    /**
     * @brief It loads all the fonts.
     */
    void LoadFonts();

public:
    /**
     * @brief Creates a new RenderWindow with given parameters.
     * @param title Title of the window.
     * @param width Width of the window.
     * @param height Height of the window.
     */
    RenderWindow(const char* title, int width, int height);

    /**
     * @brief Clears the renderer (canvas).
     */
    void Clear();

    /**
     * @brief Displays the renderer to the window.
     */
    void Display();

    /**
     * @brief Renders text to the screen with given parameters. (Not really used because it creates and destroys the texture every time).
     * @param text Text to be displayed.
     * @param x x coordinate of the text.
     * @param y y coordinate of the text.
     * @param colour Colour of the text.
     * @param font Type of the font.
     * @param language Language of the text.
     */
    void RenderText(std::string text, int x, int y, Colour colour, FontType font, Language language);

    /**
     * @brief Draws background to the screen with the given (or default) colours.
     * @param r Red.
     * @param g Green.
     * @param b Blue.
     */
    void DrawBackground(int r=166, int g=181, int b=246);

    /**
     * @brief Draws a black rectangle on the screen to simulate transition post-processing.
     * @param transparency Transparency of the transition. If 0, it does not change the screen. If 255, the screen becomes black.
     */
    void ApplyTransition(int transparency);

    /**
     * @return Returns the width of the window.
     */
    int GetWidth() const;

    /**
     * @return Returns the height of the window.
     */
    int GetHeight() const;

    /**
     * @return Returns the renderer of the window.
     */
    SDL_Renderer* GetRenderer();

    /**
     * @brief Returns the asked font. Necessary for rendering text.
     * @param font Type of the font.
     * @param language Language of the font.
     * @return Returns the wanted font.
     */
    TTF_Font* GetFont(FontType font, Language language);

    /**
     * Destructor: destroy the window, the renderer, and calls SDL_Quit().
     */
    ~RenderWindow();
};

/****************************************************************************************************/

/**
 * @brief Abstract base class for implementing UI buttons.
 */
class Button {
public:
    enum Type { NONE=0, START, EXIT, PAUSE, CONTINUE, ENG, JP, HUN, LVL1, LVL2 };   ///< Type of buttons.
    static RenderWindow* window;    ///< Static member: pointer to the window. Necessary for rendering and updating buttons.

protected:
    Type buttonType;    ///< Type of the button.
    Texture texture;    ///< Texture of the button.
    Texture selectBox;  ///< Texture of the select box around the button.
    bool isSelected;    ///< Contains whether the button is selected.
    bool isTextBased;   ///< Contains whether the button is text-based.
    const int padding;  ///< Contains padding (between the content and the backdrop).
    const int radius;   ///< Contains the radius of backdrop' rounding.

public:
    /**
     * @brief Creates a new Button withe given parameters.
     * @param buttonType Type of the button.
     * @param srcRect Source rectangle of the Texture.
     * @param destRect Destination rectangle of the Texture.
     * @param isTextBased Contains whether button is text-based.
     * @param padding Padding between the content and the backdrop.
     * @param isSelected Contains whether the button is selected.
     * @param radius Radius of the backdrop's rounding.
     */
    Button(Type buttonType, SDL_Rect srcRect, SDL_Rect destRect, bool isTextBased, int padding, bool isSelected=false, int radius=0);

    /**
     * @brief Draws button on screen.
     */
    virtual void DrawButton() = 0;

    /**
     * @brief Returns whether the button was clicked at the given position.
     * @param x x coordinate of the mouse.
     * @param y y coordinate of the mouse.
     * @return Returns if button was clicked.
     */
    bool IsClicked(int x, int y) const;

    /**
     * @return Returns whether the button is selected.
     */
    bool IsSelected() const;

    /**
     * @return Returns whether the button is text-based.
     */
    bool IsTextBased() const;

    /**
     * @return Returns whether the type of the button.
     */
    Type GetButtonType() const;

    /**
     * @brief Sets selection to given value.
     * @param selected Contains whether button is selected.
     */
    void SetSelected(bool selected);

    /**
     * Destructor.
     */
    virtual ~Button();
};

/****************************************************************************************************/

/**
 * @brief Text button derived from base class Button.
 */
class TextButton : public Button {
private:
    std::string caption;    ///< Text of the button.
    Lang::CaptionType captionType;  ///< Caption type of the button.
    SDL_Surface* surface;   ///< Surface of the button.
    FontType font;  ///< Font of the button.
    Colour colour;  ///< Colour of the button.
    const int backgroundOpacity;    ///< Background opacity of the backdrop.

    /**
     * @brief Draws the select box around the button.
     */
    void DrawSelectBox();

public:
    /**
     * @brief Creates a new TextButton withe given parameters.
     * @param buttonType Type of the button.
     * @param capType Type of the caption.
     * @param x x coordinate of the button.
     * @param y y coordinate of the button.
     * @param colour Colour of the text.
     * @param font Type of the font.
     * @param language Language of the button.
     * @param bgOpacity Opacity of the backdrop.
     * @param isSelected Contains whether the button is selected.
     */
    TextButton(Button::Type buttonType, Lang::CaptionType capType, int x, int y, Colour colour, FontType font, Language language, int bgOpacity=0, bool isSelected=false);

    /**
     * @brief Creates a new TextButton withe given parameters.
     * @param buttonType Type of the button.
     * @param caption String.
     * @param x x coordinate of the button.
     * @param y y coordinate of the button.
     * @param colour Colour of the text.
     * @param font Type of the font.
     * @param bgOpacity Opacity of the backdrop.
     * @param isSelected Contains whether the button is selected.
     * @param language Language of the button.
     */
    TextButton(Button::Type buttonType, std::string caption, int x, int y, Colour colour, FontType font, int bgOpacity=0, bool isSelected=false, Language language = ENGLISH);

    /**
     * @brief Draws button on screen.
     */
    void DrawButton();

    /**
     * @brief Updates the caption (usually when changing language).
     * @param newCaption String.
     * @param newLanguage New language.
     */
    void UpdateCaption(std::string newCaption, Language newLanguage);

    /**
     * @brief Destroys the buttons select box.
     */
    void DestroySelectBoxTexture();

    /**
     * @return Returns the type of the caption.
     */
    Lang::CaptionType GetCaptionType() const;

    /**
     * @brief Destructor: deletes the surface.
     */
    ~TextButton();
};

/****************************************************************************************************/

/**
 * @brief Image button derived from base class Button.
 */
class ImageButton : public Button {
private:
    /**
     * @brief Draws the select box around the button.
     */
    void DrawSelectBox();

public:
    /**
     * @brief d
     */
    ImageButton(Button::Type buttonType, SDL_Rect destRect, const char* path, bool isSelected=false);

    /**
     * @brief Draws button on screen.
     */
    void DrawButton();

    /**
     * @brief Destructor.
     */
    ~ImageButton();
};

/****************************************************************************************************/

/**
 * @brief Applies transition effect to the screen, thus simulating post-processing.
 */
class Transition {
private:
    Timer timer;    ///< Measures time which is necessary for applying correct transparency and knowing the state of the change.
    bool AlreadyReachedMiddle;  ///< Contains whether the transition has reached the middle of its duration.

public:
    /**
     * @brief Default constructor.
     */
    Transition() = default;

    /**
     * @brief Starts a new transition with the given duration.
     * @param milliSeconds Duration of the transition.
     */
    void SetTransition(size_t milliSeconds);

    /**
     * @brief Deactivates the transition.
     */
    void Deactivate();

    /**
     * @return Returns the current transparency of the transition.
     */
    int GetTransparency() const;

    /**
     * @return Returns how many percent of the duration has been completed.
     */
    float GetPercent() const;

    /**
     * @return Returns whether transition is active.
     */
    bool IsActive() const;

    /**
     * @return Returns whether transition has expired.
     */
    bool HasExpired() const;

    /**
     * @return Returns whether transition has reached the middle of its duration.
     */
    bool IsMiddle() const;

    /**
     * @brief Signal it to the transition that it has received that the middle of the duration has been reached.
     */
    void ReachMiddle();

    /**
     * @brief Destructor.
     */
    ~Transition();
};

#endif // CPORTA

#endif // RENDER_WINDOW_H