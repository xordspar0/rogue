#pragma once

typedef struct Room_Layout{
	int generated_rooms;
	int rooms[10][4];
	int generated_hallways;
	int hallways[9][4][2];
} Room_Layout;

typedef struct Connection{
	int hallway;
	int wall;
	int location;
	int segment;
} Connection;

typedef struct Connection_List{
	Connection connections[9];
	int count;
} Connection_List;

typedef struct Connection_Table{
	Connection_List connections[10];
	int hallway_count;
} Connection_Table;

typedef struct Floor{
	int height;
	int width;
	char out_of_bounds_character;
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

void clear_connections(Connection_Table *);
int split(int);
int split_room(Room_Layout *, int, Connection_Table *);
void generate_hallways(Room_Layout *, Connection_Table);
wall_component get_floor_element(Floor, int, int);
int floor_walkable(Floor, int, int);
void set_floor_element(Floor, int, int, wall_component);
void update_wall(Floor, int, int, wall_component);
void clear_floor(Floor);
void draw_floor(Floor);
void new_room(Floor, int, int, int, int);
void new_hallway(Floor, int[4][2], int);
void make_floor(Floor, Room_Layout);
