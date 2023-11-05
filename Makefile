CFLAGS=-Wall -Wextra -Werror -pedantic 
CLIBS=-L. `pkg-config --libs --cflags raylib` -lGL -lm -lpthread -ldl -lrt -lX11
CC=clang

SRC_DIR := ./src
OBJ_DIR := ./obj
SRC_FILES = AssList.c cJsonParser.c free.c gui.c Ll.c parsers.c

all: full

free:
	$(CC) $(CFLAGS) src/free.c -c -o $(OBJ_DIR)/$@.o
gui:
	$(CC) $(CFLAGS) src/gui.c -c -o $(OBJ_DIR)/$@.o

parsers:
	$(CC) $(CFLAGS) src/parsers.c -c -o $(OBJ_DIR)/$@.o

AssList:
	$(CC) $(CFLAGS) src/libs/AssList.c -c -o $(OBJ_DIR)/$@.o

Ll:
	$(CC) $(CFLAGS) src/libs/Ll.c -c -o $(OBJ_DIR)/$@.o

cJsonParser:
	$(CC) $(CFLAGS) src/libs/cJsonParser.c -c -o $(OBJ_DIR)/$@.o

main:
	$(CC) $(CFLAGS) src/main.c -c -o $(OBJ_DIR)/$@.o

createdir:
	mkdir -p obj

full: createdir free gui parsers AssList Ll cJsonParser main
	$(CC) $(CFLAGS) obj/main.o obj/AssList.o obj/cJsonParser.o obj/free.o obj/gui.o obj/Ll.o obj/parsers.o -o gyagey $(CLIBS)

link:
	$(CC) $(CFLAGS) src/main.c obj/AssList.o obj/cJsonParser.o obj/free.o obj/gui.o obj/Ll.o obj/parsers.o -o gyagey $(CLIBS)

cross:
	x86_64-w64-mingw32-gcc $(CFLAGS) main.c -o gyagey.exe $(CLIBS)

run:
	./gyagey game.json

clean:
	rm ./obj/*
