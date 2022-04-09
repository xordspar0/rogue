deps = sdl2 ncurses
CFLAGS = -Wall -g -pedantic $(shell pkg-config --cflags $(deps)) -ISDL_sound/src
LDLIBS = $(shell pkg-config --libs $(deps)) SDL_sound/build/libSDL2_sound.a

rogue: rogue.o player.o floor.o math.o monsters/giant_rat.o sound.o
rogue.o: rogue.c floor.o player.o monsters/giant_rat.o
sound.o: sound.c sound.h SDL_sound/build/libSDL2_sound.a
floor.o: floor.c floor.h math.c math.h
player.o: player.c player.h
monsters/giant_rat.o: monsters/giant_rat.c monsters/giant_rat.h

SDL_sound/build/libSDL2_sound.a: SDL_sound/build
	$(MAKE) -C $<

SDL_sound/build:
	mkdir -p $@
	cmake -S SDL_sound -B $@

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
