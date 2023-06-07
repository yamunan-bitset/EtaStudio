CC = g++
SRC = imgui_sdl.o eta.o main.o # TODO: compile eta.cc into static library
LINK = -lSDL2 -lSDL2_gfx -lSDL2_ttf -limgui -lstb
TARGET = example

all: $(SRC)
	$(CC) $(SRC) $(LINK) -o $(TARGET)

clean:
	rm -rf $(TARGET) $(SRC)
