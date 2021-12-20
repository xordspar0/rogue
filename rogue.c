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

	Room_Layout layout;
	layout.generated_rooms = 1;
	layout.rooms[0][0] = 1;
	layout.rooms[0][1] = 1;
	layout.rooms[0][2] = width - 2;
	layout.rooms[0][3] = height - 2;
	layout.generated_hallways = 0;

	split_room(&layout, 0);
	split_room(&layout, 0);
	split_room(&layout, 1);
	split_room(&layout, 0);
	split_room(&layout, 1);
	split_room(&layout, 2);
	split_room(&layout, 1);


	Floor floor;
	floor.height = height;
	floor.width = width;

	char floor_background_array[width][height];
	floor.array = (char *)floor_background_array;
	clear_floor(floor);

	make_floor(floor, layout);

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
