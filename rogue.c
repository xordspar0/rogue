#include <curses.h>
#include <stdlib.h>

#include "player.h"
#include "monsters/giant_rat.h"

void print_int(int i)
{
	char buffer[20];
	sprintf(buffer, "%d", i);
	mvaddstr(0, 0, "                         ");
	mvaddstr(0, 0, buffer);
}

typedef struct Floor{
	int height;
	int width;
	char * array;
} Floor;

typedef enum wall_component{
	wall_ept = 0,
	wall_ver,
	wall_hor,
	wall_inc,
	wall_ouc,
	wall_flr,
} wall_component;

char wall_symbol[] = {
	' ',
	'|',
	'-',
	'+',
	'+',
	'.',
	'?',
};

wall_component get_floor_element(Floor floor, int x, int y)
{
	return floor.array[x * floor.height + y];
}

void set_floor_element(Floor floor, int x, int y, wall_component c)
{
	floor.array[x * floor.height + y] = c;
}

void update_wall(Floor floor, int x, int y, wall_component wall_component)
{
	char prev = get_floor_element(floor, x, y);
	if (prev > wall_ouc) return;
	if ( (prev == wall_ouc) | (prev == wall_ept)) {
		set_floor_element(floor, x, y, wall_component);
	}
	else {
		if (wall_component == wall_ouc) return;
		set_floor_element(floor, x, y, prev | wall_component);
	}
}

void clear_floor(Floor floor)
{
	for (int x = 0; x < floor.width; x++) {
		for (int y = 0; y < floor.height; y++) {
			set_floor_element(floor, x, y, wall_ept);
		}
	}
}

void draw_floor(Floor floor) {
	for (int x = 0; x < floor.width; x++) {
		for (int y = 0; y < floor.height; y++) {
			mvaddch(y, x, wall_symbol[get_floor_element(floor, x, y)]);
		}
	}
}

void new_room(Floor floor, int x, int y, int width, int height)
{
	update_wall(floor, x - 1, y - 1, wall_ouc);
	update_wall(floor, x + width, y - 1, wall_ouc);
	update_wall(floor, x - 1, y + height, wall_ouc);
	update_wall(floor, x + width, y + height, wall_ouc);
	for(int i = 0; i < width; i++)
	{
		update_wall(floor, x + i, y - 1, wall_hor);
		update_wall(floor, x + i, y + height, wall_hor);
	}


	 for (int k = 0; k < height; k++) {
	 	for (int l = 0; l < width; l++) {
			set_floor_element(floor, l + x, k + y, wall_flr);

		}
		update_wall(floor, x - 1, y + k, wall_ver);
		update_wall(floor, x + width, y + k, wall_ver);
	}
}

int min(int a, int b)
{
	return a < b ? a : b;
}

int max(int a, int b)
{
	return a > b ? a : b;
}

void new_hallway(Floor floor, int points[][2], int length)
{
	int start[2] = {points[0][0], points[0][1]};
	for (int i = 1; i < length; i++) {
		int end[2] = {start[0] + points[i][0], start[1] + points[i][1]};
		new_room(floor,
			min(start[0],end[0]),
			min(start[1],end[1]),
			abs(points[i][0])+1,
			abs(points[i][1])+1);
		start[0] = end[0];
		start[1] = end[1];
	}
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
	floor.array = (char *) floor_background_array;
	clear_floor(floor);
	new_room(floor, 5, 2, 7, 5);
	new_room(floor, 25, 22, 7, 5);
	new_hallway(floor, (int[][2]){{8,7}, {0,10}, {10,0}, {0,-5}, {-3, 0}}, 5);

	player p = {10, 5};
	monster r = {20, 20, 'r', *giant_rat_update};
	for (int c = 0; c != 'q'; c = getch()) {
		player_input(&p, c);
		r.update(&r, p);

		draw_floor(floor);
		player_draw(&p);
		monster_draw(&r);
	}

	endwin();
}
