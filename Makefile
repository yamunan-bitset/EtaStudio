CC = cc
SRC = main.c
LIBS = -lSDL2 -lSDL2_gfx -lSDL2_ttf -leta -Lbuild
TARGET = -o 3body

all:
	$(CC) $(SRC) $(LIBS) $(TARGET)
