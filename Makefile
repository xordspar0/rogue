rogue: rogue.o player.o monsters/giant_rat.o
	cc $^ -o rogue -l curses

player.o: player.c player.h

monsters/giant_rat.o: monsters/giant_rat.c monsters/giant_rat.h
