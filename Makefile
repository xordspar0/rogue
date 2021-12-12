CFLAGS = -Wall -g -pedantic $(shell pkg-config --cflags opusfile)

rogue: rogue.o player.o floor.o math.o monsters/giant_rat.o sound.o
	cc $(CFLAGS) $^ -o rogue -l curses $(shell pkg-config --libs portaudio-2.0 opusfile opus)

rogue.o: rogue.c floor.o player.o monsters/giant_rat.o
sound.o: sound.c sound.h
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
