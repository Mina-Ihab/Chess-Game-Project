CC = gcc
MAIN = ./src/main
SYSTEM = ./src/system
BOARD = ./src/board

Run: Build
	./bin/game.exe

Build: Compile
	$(CC) ./bin/main.o ./bin/board.o ./bin/system.o -o ./bin/game.exe

Compile: ./src/main.c ./src/board.c ./src/system.c
	$(CC) -c $(MAIN).c -o ./bin/main.o
	$(CC) -c $(BOARD).c -o ./bin/board.o
	$(CC) -c $(SYSTEM).c -o ./bin/system.o

	