# Cat Mario :scream_cat:

This game is written in C++ and uses SDL2 library. <br>
Not only does it have a wonderful menu, but it also supports three languages! 🇬🇧 🇯🇵 🇭🇺 <br>
Furthermore, you're presented with deep philosophical quotes that hinder you from rage quitting :sweat_smile:

## Install SDL2 on Linux (Debian):
``` bash
sudo apt install libsdl2-dev libsdl2-gfx-dev libsdl2-image-dev libsdl2-ttf-dev libsdl2-mixer-dev
```

## Compiled program is available [here](https://drive.google.com/drive/folders/1bmBtZN4KHAPi11nzDXE6cC0na3l_qFhV?usp=drive_link).

## Build and run in VS Code
Select main.cpp and click on the triangle (Run C/C++ File).

## Compile manually with g++
In the project directory:
```bash
g++ -std=c++17 -I ./include ./src/*.cpp -o ./bin/Cat-Mario -lSDL2 -lSDL2_gfx -lSDL2_ttf -lSDL2_image -lSDL2_mixer -Wall -Werror
```
