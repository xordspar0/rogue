CFLAGS = -Wall -g -pedantic

rogue: rogue.o player.o floor.o math.o monsters/giant_rat.o
	cc $(CFLAGS) $^ -o rogue -l curses

rogue.o: rogue.c floor.o player.o monsters/giant_rat.o
floor.o: floor.c floor.h math.c math.h
player.o: player.c player.h
monsters/giant_rat.o: monsters/giant_rat.c monsters/giant_rat.h

.PHONY: time
time:
	awk '{sum += $$2} END { print sum }' time.txt

.PHONY: fmt
fmt:
	find . -name '*.c' | xargs env VERSION_CONTROL=none indent -linux

.PHONY: clean
clean:
	rm -f *.o
	rm -f rogue
