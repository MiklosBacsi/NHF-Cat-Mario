#ifndef INPUT_H
#define INPUT_H

class Input {
private:
    int MouseX;
    int MouseY;
    bool MouseClick;
    bool Esc;
    bool W;
    bool A;
    bool S;
    bool D;
    bool P;
    bool Space;
public:
    Input();
    int GetMouseX() const;
    int GetMouseY() const;
    bool GetMouseClick() const;
    bool GetEsc() const;
    bool GetUp() const; // W
    bool GetLeft() const;  // A
    bool GetDown() const;  // S
    bool GetRight() const;  // D
    bool GetPause() const;
    bool GetSpace() const;
    void SetMouseX(int x);
    void SetMouseY(int y);
    void SetMouseClick(bool pressed);
    void SetEsc(bool pressed);
    void SetW(bool pressed);
    void SetA(bool pressed);
    void SetS(bool pressed);
    void SetD(bool pressed);
    void SetP(bool pressed);
    void SetSpace(bool pressed);
    ~Input();
};

#endif // INPUT_H