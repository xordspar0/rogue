#include <curses.h>

#include "floor.h"
#include "monsters/giant_rat.h"
#include "player.h"

void print_int(int i)
{
	char buffer[20];
	sprintf(buffer, "%d", i);
	mvaddstr(0, 0, "                         ");
	mvaddstr(0, 0, buffer);
}

int main(void)
{
	initscr();
	cbreak();
	noecho();
	curs_set(0);

	keypad(stdscr, TRUE);

	int height, width = 0;
	getmaxyx(stdscr, height, width);

	Floor floor;
	floor.height = height;
	floor.width = width;

	char floor_background_array[width][height];
	floor.array = (char *)floor_background_array;
	clear_floor(floor);
	new_room(floor, 5, 2, 7, 5);
	new_room(floor, 25, 22, 7, 5);
	new_hallway(floor, (int[][2]) { {8, 7}, {0, 10}, {10, 0}, {0, -5},
		    {-3, 0}
		    }, 5);

	player p = { 10, 5 };
	monster r = { 27, 24, 'r', *giant_rat_update };
	for (int c = 0; c != 'q'; c = getch()) {
		player_input(floor, &p, c);
		r.update(floor, &r, p);

		draw_floor(floor);
		monster_draw(&r);
		player_draw(&p);
	}

	endwin();
}
