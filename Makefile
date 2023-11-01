CFLAGS=-Wall -Wextra -Werror -pedantic 
CLIBS=-L. `pkg-config --libs --cflags raylib` -lGL -lm -lpthread -ldl -lrt -lX11
CC=clang
all: compile

compile:
	$(CC) $(CFLAGS) main.c -o gyagey $(CLIBS)

cross:
	x86_64-w64-mingw32-gcc $(CFLAGS) main.c -o gyagey.exe $(CLIBS)

run: compile
	./gyagey game.json
