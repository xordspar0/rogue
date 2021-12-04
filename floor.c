#include <curses.h>
#include <stdlib.h>

#include "floor.h"
#include "math.h"

static char wall_symbol[] = {
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

int floor_walkable(Floor floor, int x, int y)
{
	char c = floor.array[x * floor.height + y];

	return c == wall_flr;
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
