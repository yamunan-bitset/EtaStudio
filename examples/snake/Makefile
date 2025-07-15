CC = g++
OBJ = ../../src/*.o
FILE = snake.cpp
LIBS = -l SDL2 -l SDL2_ttf -l SDL2_image -l SDL2_gfx -l imgui -l stb
OUT = snake

build:
	$(CC) $(OBJ) $(FILE) $(LIBS) -o $(OUT)

clean:
	rm -rf *.o *.out *.exe $(OUT)
