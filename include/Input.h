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
    int getMouseX() const;
    int getMouseY() const;
    bool getMouseClick() const;
    bool getEsc() const;
    bool getUp() const; // W
    bool getLeft() const;  // A
    bool getDown() const;  // S
    bool getRight() const;  // D
    bool getPause() const;
    bool getSpace() const;
    void setMouseX(int x);
    void setMouseY(int y);
    void setMouseClick(bool pressed);
    void setEsc(bool pressed);
    void setW(bool pressed);
    void setA(bool pressed);
    void setS(bool pressed);
    void setD(bool pressed);
    void setP(bool pressed);
    void setSpace(bool pressed);
    ~Input();
};

#endif // INPUT_H