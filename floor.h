#pragma once

typedef struct Room_Layout{
	int generated_rooms;
	int rooms[10][4];
} Room_Layout;

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

int split(int);
int split_room(Room_Layout *);
wall_component get_floor_element(Floor, int, int);
int floor_walkable(Floor, int, int);
void set_floor_element(Floor, int, int, wall_component);
void update_wall(Floor, int, int, wall_component);
void clear_floor(Floor);
void draw_floor(Floor);
void new_room(Floor, int, int, int, int);
void new_hallway(Floor, int[][2], int);
void make_floor(Floor, Room_Layout);
