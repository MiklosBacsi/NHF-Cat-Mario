#include <iostream>

#include "Input.h"

Input::Input() : MouseX(0), MouseY(0), MouseClick(false), Esc(false),
    W(false), A(false), S(false), D(false), P(false), Space(false) {}

int Input::getMouseX() const { return MouseX; }

int Input::getMouseY() const { return MouseY; }

bool Input::getMouseClick() const { return MouseClick; }

bool Input::getEsc() const { return Esc; }

bool Input::getUp() const { return W; }

bool Input::getLeft() const { return A; }

bool Input::getDown() const { return S; }

bool Input::getRight() const { return D; }

bool Input::getPause() const { return P; }

bool Input::getSpace() const { return Space; }

void Input::setMouseX(int x) { MouseX = x; }

void Input::setMouseY(int y) { MouseY = y; }

void Input::setMouseClick(bool pressed) { MouseClick = pressed; }

void Input::setEsc(bool pressed) { Esc = pressed; }

void Input::setW(bool pressed) { W = pressed; }

void Input::setA(bool pressed) { A = pressed; }

void Input::setS(bool pressed) { S = pressed; }

void Input::setD(bool pressed) { D = pressed; }

void Input::setP(bool pressed) { P = pressed; }

void Input::setSpace(bool pressed) { Space = pressed; }

Input::~Input() {
    std::cout << "~Input Dtor" << std::endl;
}