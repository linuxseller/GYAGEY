CFLAGS=-Wall -Wextra -Werror -pedantic
CLIBS=-L. `pkg-config --libs raylib` -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
CC=clang
all: compile

compile:
	$(CC) $(CFLAGS) main.c -o gyagey $(CLIBS)

run: compile
	./gyagey game.json
