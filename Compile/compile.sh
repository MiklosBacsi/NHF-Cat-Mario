clear
g++ -fdiagnostics-color=always -g -fsanitize=address -std=c++17 -I ../include ../src/*.cpp -o ../bin/Cat-Mario -lSDL2 -lSDL2_gfx -lSDL2_ttf -lSDL2_image -lSDL2_mixer -Wall -Werror
LSAN_OPTIONS=verbosity=1:log_threads=1 ../bin/Cat-Mario