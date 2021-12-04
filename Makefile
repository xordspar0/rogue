CFLAGS = -Wall -g -pedantic

rogue: rogue.o player.o floor.o math.o monsters/giant_rat.o
	cc $(CFLAGS) $^ -o rogue -l curses

player.o: player.c player.h
monsters/giant_rat.o: monsters/giant_rat.c monsters/giant_rat.h
floor.o: floor.c floor.h

.PHONY: clean
clean:
	rm -f *.o
	rm -f rogue
