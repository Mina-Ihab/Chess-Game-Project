CC = gcc
MAIN = ./src/main
SYSTEM = ./src/system
BOARD = ./src/board
MOVE = ./src/move

Run: Build
	./bin/game.exe

Build: Compile
	$(CC) ./bin/main.o ./bin/board.o ./bin/system.o ./bin/move.o -o ./bin/game.exe

Compile: ./src/main.c ./src/board.c ./src/system.c ./src/move.c
	$(CC) -c $(MAIN).c -o ./bin/main.o
	$(CC) -c $(BOARD).c -o ./bin/board.o
	$(CC) -c $(SYSTEM).c -o ./bin/system.o
	$(CC) -c $(MOVE).c -o ./bin/move.o