#include <iostream>

#include "Input.h"

Input::Input() : MouseX(0), MouseY(0), MouseClick(false), Esc(false),
    W(false), A(false), S(false), D(false), P(false), Space(false) {}

int Input::GetMouseX() const { return MouseX; }

int Input::GetMouseY() const { return MouseY; }

bool Input::GetMouseClick() const { return MouseClick; }

bool Input::GetEsc() const { return Esc; }

bool Input::GetUp() const { return W; }

bool Input::GetLeft() const { return A; }

bool Input::GetDown() const { return S; }

bool Input::GetRight() const { return D; }

bool Input::GetPause() const { return P; }

bool Input::GetSpace() const { return Space; }

void Input::SetMouseX(int x) { MouseX = x; }

void Input::SetMouseY(int y) { MouseY = y; }

void Input::SetMouseClick(bool pressed) { MouseClick = pressed; }

void Input::SetEsc(bool pressed) { Esc = pressed; }

void Input::SetW(bool pressed) { W = pressed; }

void Input::SetA(bool pressed) { A = pressed; }

void Input::SetS(bool pressed) { S = pressed; }

void Input::SetD(bool pressed) { D = pressed; }

void Input::SetP(bool pressed) { P = pressed; }

void Input::SetSpace(bool pressed) { Space = pressed; }

bool& Input::DisableEsc() { return disableEsc; }

Input::~Input() {
    #ifdef DTOR
    std::clog << "~Input Dtor" << std::endl;
    #endif
}