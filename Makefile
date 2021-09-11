rogue: rogue.o player.o
	cc $^ -o rogue -l curses

player.o: player.c player.h
