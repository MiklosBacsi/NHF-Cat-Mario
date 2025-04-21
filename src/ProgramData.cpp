#include <SDL2/SDL.h>
#include <iostream>

#include "ProgramData.h"
#include "RenderWindow.h"

ProgramData::ProgramData() {}

ProgramData::~ProgramData() {
    std::cout << "~ProgramData Dtor" << std::endl;
}