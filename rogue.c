#include <curses.h>

#include "floor.h"
#include "monsters/giant_rat.h"
#include "player.h"
#include "sound.h"

void print_int(int i)
{
	char buffer[20];
	sprintf(buffer, "%d", i);
	mvaddstr(0, 0, "                         ");
	mvaddstr(0, 0, buffer);
}

int split(int a)
{
	return a / 2;
}

int main(void)
{
	initscr();
	cbreak();
	noecho();
	curs_set(0);

	keypad(stdscr, TRUE);

	sound_init();
	sound_play(0);

	int height, width = 0;
	getmaxyx(stdscr, height, width);

	Floor floor;
	floor.height = height;
	floor.width = width;

	char floor_background_array[width][height];
	floor.array = (char *)floor_background_array;
	clear_floor(floor);

	int a = split(height);
	new_room(floor, 1, 1, width - 2, a - 1);
	int b = split(width);
	new_room(floor, 1, a + 1, b - 1, height - 2);
	new_room(floor, b + 1, a + 1, width - 2, height - 2);
	// new_room(floor, 25, 22, 7, 5);
	/* new_hallway(
	   floor,
	   (int[][2]) {
	   {6, 14},
	   {1, 10},
	   {10, 0},
	   {0, -5},
	   {-3, 0}
	   },
	   5); */

	player p = { 10, 5 };
	monster r = { 34, 3, 'r', *giant_rat_update };
	for (int c = 0; c != 'q'; c = getch()) {
		player_input(floor, &p, c);
		r.update(floor, &r, p);

		draw_floor(floor);
		monster_draw(&r);
		player_draw(&p);
	}

	endwin();
}
