CFLAGS=-Wall -Wextra -Werror -pedantic

ifeq ($(TARGET), linux)
	CLIBS=-L. `pkg-config --libs --cflags raylib` -lGL -lm -lpthread -ldl -lrt -lX11
	CC:=clang
else
	CLIBS=-L./raylib-4.5.0_win64_mingw-w64/lib/  -lraylib -lopengl32 -lgdi32 -lwinmm -static
	INCLUDE=-I./raylib-4.5.0_win64_mingw-w64/include/ -mwindows
	CC:=x86_64-w64-mingw32-gcc
endif



SRC = ./src
OBJ = ./obj
SRC_FILES = AssList.c cJsonParser.c free.c gui.c Ll.c parsers.c

SOURCES := $(wildcard $(SRC)/*.c) $(wildcard $(SRC)/libs/*.c)
OBJECTS := $(patsubst $(SRC)/%.c, $(OBJ)/%.o, $(SOURCES))

all: full
	echo  clang tcc gcc

$(OBJ)/%.o: $(SRC)/%.c
	$(CC) $(INCLUDE) $(CFLAGS) -c $< -o $@

createdir:
	mkdir -p obj/libs

full:
	$(CC) $(CFLAGS) obj/main.o obj/AssList.o obj/free.o obj/gui.o obj/Ll.o obj/parsers.o -o gyagey $(CLIBS)

link:
	$(CC) $(CFLAGS) $(OBJECTS) main.c -o gyagey $(CLIBS)

full: createdir $(OBJECTS)
	$(CC) $(OBJECTS) $(CLIBS) -o gyagey

cross: $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) main.c -o gyagey $(CLIBS)

	
run:
	./gyagey game.json

clean:
	rm ./obj/*
