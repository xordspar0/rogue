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
	split_room(&layout, 2);
	split_room(&layout, 0);
	split_room(&layout, 3);
	split_room(&layout, 2);
	split_room(&layout, 2);

	Floor floor;
	floor.height = height;
	floor.width = width;

	char floor_background_array[width][height];
	floor.array = (char *)floor_background_array;
	clear_floor(floor);

	make_floor(floor, layout);

	player p = { 10, 5 };
	/*monster monsters[]

	 */
	monster r = { 32, 3, 'r', *giant_rat_update };
	monster r2 = { 32, 5, 'r', *giant_rat_update };
	for (int c = 0; c != 'q'; c = getch()) {
		r.update(floor, &r, p);
		r2.update(floor, &r2, p);
		player_input(floor, &p, c);

		draw_floor(floor);
		monster_draw(&r);
		monster_draw(&r2);
		player_draw(&p);
	}

	endwin();
}
