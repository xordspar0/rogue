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

int split(int a)
{
	return a / 2;
}

int split_room(Room_Layout * layout)
{
	int new_index = layout->generated_rooms;
	if (new_index > 10) {
		return 0;
	}
	layout->generated_rooms++;

	int r0x0 = layout->rooms[0][0];
	int r0y0 = layout->rooms[0][1];
	int r0x1;
	int r0y1;
	int r1x0;
	int r1y0;
	int r1x1 = layout->rooms[0][2];
	int r1y1 = layout->rooms[0][3];

	if (r1x1 - r0x0 < r1y1 - r0y0) {
		r0x1 = r1x1;
		r0y1 = split(r0y0 + r1y1);
		r1x0 = r0x0;
		r1y0 = r0y1;
		r0y1--;
		r1y0++;
	} else {
		r0x1 = split(r0x0 + r1x1);
		r0y1 = r1y1;
		r1x0 = r0x1;
		r1y0 = r0y0;
		r0x1--;
		r1x0++;
	}

	layout->rooms[0][2] = r0x1;
	layout->rooms[0][3] = r0y1;
	layout->rooms[new_index][0] = r1x0;
	layout->rooms[new_index][1] = r1y0;
	layout->rooms[new_index][2] = r1x1;
	layout->rooms[new_index][3] = r1y1;

	return 0;
}

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
	if (prev > wall_ouc)
		return;
	if ((prev == wall_ouc) | (prev == wall_ept)) {
		set_floor_element(floor, x, y, wall_component);
	} else {
		if (wall_component == wall_ouc)
			return;
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

void draw_floor(Floor floor)
{
	for (int x = 0; x < floor.width; x++) {
		for (int y = 0; y < floor.height; y++) {
			mvaddch(y, x,
				wall_symbol[get_floor_element(floor, x, y)]);
		}
	}
}

void new_room(Floor floor, int x0, int y0, int x1, int y1)
{
	update_wall(floor, x0 - 1, y0 - 1, wall_ouc);
	update_wall(floor, x1 + 1, y0 - 1, wall_ouc);
	update_wall(floor, x1 + 1, y1 + 1, wall_ouc);
	update_wall(floor, x0 - 1, y1 + 1, wall_ouc);
	for (int i = x0; i <= x1; i++) {
		update_wall(floor, i, y0 - 1, wall_hor);
		update_wall(floor, i, y1 + 1, wall_hor);
	}

	for (int k = y0; k <= y1; k++) {
		for (int l = x0; l <= x1; l++) {
			set_floor_element(floor, l, k, wall_flr);

		}
		update_wall(floor, x0 - 1, k, wall_ver);
		update_wall(floor, x1 + 1, k, wall_ver);
	}
}

void new_hallway(Floor floor, int points[][2], int length)
{
	int start[2] = { points[0][0], points[0][1] };
	for (int i = 1; i < length; i++) {
		int end[2] =
		    { start[0] + points[i][0], start[1] + points[i][1] };
		new_room(floor, min(start[0], end[0]), min(start[1], end[1]),
			 max(start[0], end[0]), max(start[0], end[0]));
		start[0] = end[0];
		start[1] = end[1];
	}
}

void make_floor(Floor floor, Room_Layout layout)
{
	for (int i = 0; i < layout.generated_rooms; i++) {
		int *room = layout.rooms[i];
		new_room(floor, room[0], room[1], room[2], room[3]);
	}
}
