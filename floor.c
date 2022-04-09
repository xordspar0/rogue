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

void clear_connections(Connection_Table * connection_table)
{
	for (int i = 0; i < 10; i++) {
		for (int j = 0; j < 9; j++) {
			connection_table->connections[i].connections[j] =
			    (Connection) {
			-1, -1};
		}
		connection_table->connections[i].count = 0;
	}
}

int split(int a)
{
	return a / 2;
}

int split_room(Room_Layout * layout, int index,
	       Connection_Table * connection_table)
{
	int new_index = layout->generated_rooms++;
	if (new_index > 10 || index >= new_index) {
		return 0;
	}

	int r0x0 = layout->rooms[index][0];
	int r0y0 = layout->rooms[index][1];
	int r0x1;
	int r0y1;
	int r1x0;
	int r1y0;
	int r1x1 = layout->rooms[index][2];
	int r1y1 = layout->rooms[index][3];

	int split_dir = r1x1 - r0x0 < r1y1 - r0y0;
	int wall_a = split_dir | 2;
	int wall_b = split_dir;
	int hallway_location = 0;
	if (split_dir) {
		r0x1 = r1x1;
		r0y1 = split(r0y0 + r1y1);
		r1x0 = r0x0;
		r1y0 = r0y1;

		hallway_location = split(r0x0 + r1x1) - 2;

		r0y1--;
		r1y0++;
		r0y1--;
		r1y0++;

	} else {
		r0x1 = split(r0x0 + r1x1);
		r0y1 = r1y1;
		r1x0 = r0x1;
		r1y0 = r0y0;

		hallway_location = split(r0y0 + r1y1) - 2;

		r0x1--;
		r1x0++;
		r0x1--;
		r1x0++;
	}

	layout->rooms[index][2] = r0x1;
	layout->rooms[index][3] = r0y1;
	layout->rooms[new_index][0] = r1x0;
	layout->rooms[new_index][1] = r1y0;
	layout->rooms[new_index][2] = r1x1;
	layout->rooms[new_index][3] = r1y1;

	Connection_List *connections_a = &connection_table->connections[index];
	Connection_List *connections_b =
	    &connection_table->connections[new_index];
	int new_count = 0;
	int move_wall = split_dir | 2;
	for (int i = 0; i < connections_a->count; i++) {
		Connection temporary_connection = connections_a->connections[i];
		connections_a->connections[i] = (Connection) {
		-1, -1};
		if (temporary_connection.wall == move_wall) {
			connections_b->connections[i] = temporary_connection;
		} else {
			connections_a->connections[new_count++] =
			    temporary_connection;
		}
	}

	int new_hallway_index = connection_table->hallway_count++;
	connections_a->connections[connections_a->count++] = (Connection){
		.hallway = new_hallway_index,
		.wall = wall_a,
		.location = hallway_location,
		.segment = 0
	};
	connections_b->connections[connections_b->count++] = (Connection){
		.hallway = new_hallway_index,
		.wall = wall_b,
		.location = hallway_location,
		.segment = 1
	};

	return 0;
}

void generate_hallways(Room_Layout * layout, Connection_Table connection_table)
{
	layout->generated_hallways++;
	for (int i = 0; i < 10; i++) {
		for (int j = 0; j < 9; j++) {
			Connection connection =
			    connection_table.connections[i].connections[j];
			if (connection.hallway == 0) {
				if (connection.segment == 0) {
					layout->hallways[connection.hallway][0][0] =
					    (layout->rooms[i][0] +
					     layout->rooms[i][2]) / 2;
					layout->hallways[connection.hallway][0][1] =
					    (layout->rooms[i][1] +
					     layout->rooms[i][3]) / 2;
					layout->hallways[connection.hallway][1][0] =
					    (layout->rooms[i][0] +
					     layout->rooms[i][2]) / 2;
					layout->hallways[connection.hallway][1][1] =
					    (layout->rooms[i][1] +
					     layout->rooms[i][3]) / 2;
					if (connection.wall & 1) {
						layout->hallways[connection.hallway][1][1] =
							connection.location;
					} else {
						layout->hallways[connection.hallway][1][0] =
							connection.location;
					}
				} else {
					layout->hallways[connection.hallway][2][0] =
					    (layout->rooms[i][0] +
					     layout->rooms[i][2]) / 2;
					layout->hallways[connection.hallway][2][1] =
					    (layout->rooms[i][1] +
					     layout->rooms[i][3]) / 2;
					layout->hallways[connection.hallway][3][0] =
					    (layout->rooms[i][0] +
					     layout->rooms[i][2]) / 2;
					layout->hallways[connection.hallway][3][1] =
					    (layout->rooms[i][1] +
					     layout->rooms[i][3]) / 2;
					if (connection.wall & 1) {
						layout->hallways[connection.hallway][2][1] =
							connection.location;
					} else {
						layout->hallways[connection.hallway][2][0] =
							connection.location;
					}
				}
			}
		}
		// connection_table->connections[i].count = 0;
	}
}

char *get_tile(Floor floor, int x, int y)
{
	char oob = 0;
	oob |= x < 0;
	oob |= y < 0;
	oob |= x >= floor.width;
	oob |= y >= floor.height;

	char *tile = &floor.out_of_bounds_character;
	if (!oob) {
		tile = &floor.array[x * floor.height + y];
	}
	return tile;
}

wall_component get_floor_element(Floor floor, int x, int y)
{
	return *get_tile(floor, x, y);
}

int floor_walkable(Floor floor, int x, int y)
{
	char c = floor.array[x * floor.height + y];

	return c == wall_flr;
}

void set_floor_element(Floor floor, int x, int y, wall_component c)
{
	*get_tile(floor, x, y) = c;
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

void new_room(Floor floor, int xa, int ya, int xb, int yb)
{

	int x0 = min(xa, xb);
	int x1 = max(xa, xb);
	int y1 = max(ya, yb);
	int y0 = min(ya, yb);

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

void new_hallway(Floor floor, int points[4][2], int length)
{
	int start_x = points[0][0];
	int start_y = points[0][1];
	for (int i = 1; i < length; i++) {
		int end_x = points[i][0];
		int end_y = points[i][1];
		new_room(floor, start_x, start_y, end_x, end_y);
		start_x = end_x;
		start_y = end_y;
	}
}

void make_floor(Floor floor, Room_Layout layout)
{
	for (int i = 0; i < layout.generated_rooms; i++) {
		int *room = layout.rooms[i];
		new_room(floor, room[0], room[1], room[2], room[3]);
	}

	for (int i = 0; i < layout.generated_hallways; i++) {
		new_hallway(floor, layout.hallways[i], 4);
	}
}
