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

enum Colour { WHITE=0, BLACK };
enum FontType { REG30=0, BOLD100, MED50, MED15, FONT_COUNT=4 };

SDL_Color GetColour(Colour colour);
int GetRadiusFromFont(FontType font);

class Font {
private:
    std::vector<TTF_Font*> fonts;
public:
    Font();
    void LoadFont(const char* path, int size, FontType fontType);
    TTF_Font* GetFont(FontType font);
    ~Font();
};

class RenderWindow {
private:
    SDL_Window* window;
    int width, height;
    Font latinFonts;
    Font japaneseFonts;

    void LoadFonts();
public:
    RenderWindow(const char* title, int width, int height);
    void Clear();
    void Display();
    void RenderText(std::string text, int x, int y, Colour colour, FontType font, Language language);
    void DrawBackground(int r=166, int g=181, int b=246);
    void ApplyTransition(int transparency);
    int GetWidth() const;
    int GetHeight() const;
    SDL_Renderer* GetRenderer();
    TTF_Font* GetFont(FontType font, Language language);

    static SDL_Renderer* renderer;
    ~RenderWindow();
};

class Button {
public:
    enum Type { NONE=0, START, EXIT, PAUSE, CONTINUE, ENG, JP, HUN, LEV1, LEV2 };
    static RenderWindow* window;
protected:
    Type buttonType;
    Texture texture;
    Texture selectBox;
    bool isSelected;
    bool isTextBased;
    const int padding;
    const int radius;
public:
    Button(Type type, SDL_Rect srcRect, SDL_Rect destRect, bool isTextBased, int padding, bool isSelected=false, int radius=0);
    virtual void DrawButton() = 0;
    bool IsClicked(int x, int y) const;
    bool IsSelected() const;
    bool IsTextBased() const;
    Type GetButtonType() const;
    void SetSelected(bool selected);
    virtual ~Button();
};

class TextButton : public Button {
private:
    std::string caption;
    Lang::CaptionType captionType;
    SDL_Surface* surface;
    FontType font;
    Colour colour;
    const int backgroundOppacity;

    void DrawSelectBox();
public:
    TextButton(Button::Type buttonType, Lang::CaptionType capType, int x, int y, Colour colour, FontType font, Language language, int bgOpacity=0, bool isSelected=false);
    TextButton(Button::Type buttonType, std::string capiton, int x, int y, Colour colour, FontType font, int bgOpacity=0, bool isSelected=false);
    void DrawButton();
    void UpdateCaption(std::string newCaption, Language newLanguage); // when changing Language
    void DestroySelectBoxTexture();
    Lang::CaptionType GetCaptionType() const;
    ~TextButton();
};

class ImageButton : public Button {
private:
    void DrawSelectBox();
public:
    ImageButton(Button::Type buttonType, SDL_Rect destRect, const char* path, bool isSelected=false);
    void DrawButton();
    ~ImageButton();
};

class Transition {
private:
    Timer timer;
    bool AlreadyReachedMiddle;
public:
    Transition() {}
    void SetTransition(size_t miliSeconds);
    void Deactivate();
    int GetTransparency() const;
    float GetPercent() const;
    bool IsActive() const;
    bool HasExpired() const;
    bool IsMiddle() const;
    void ReachMiddle();
    ~Transition();
};

#endif // CPORTA

#endif // RENDER_WINDOW_H