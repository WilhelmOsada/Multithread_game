all: player server

player: player.h source.h player.cpp
	g++ player.cpp -o player -lncursesw -pthread

server: server.h source.h server.cpp
	g++ server.cpp -o server -lncursesw -pthread

.PHONY: clean check debug

clean:
	-rm server

check: game
	valgrind ./game

debug: game
	gdb game