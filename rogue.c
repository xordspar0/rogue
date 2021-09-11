#include <curses.h>

#include "player.h"
#include "monsters/giant_rat.h"

void print_int(int i)
{
	char buffer[20];
	sprintf(buffer, "%d", i);
	mvaddstr(0, 0, "                         ");
	mvaddstr(0, 0, buffer);
}

void draw_room(int x, int y, int width, int height)
{
	mvaddstr(y, x, "+");
	mvaddstr(y, x + width, "+");
	mvaddstr(y + height, x, "+");
	mvaddstr(y + height, x + width, "+");
	for(int i = 1; i < width; i++)
	{
		mvaddstr(y, x + i, "-");
	}

	for(int j = 1; j < width; j++)
	{
		mvaddstr(y + height, x + j, "-");
	}


	for(int k = 1; k < height; k++)
	{
		mvaddstr(y + k, x, "|");
		mvaddstr(y + k, x + width, "|");
		for(int l = 1; l < width; l++)
		{
			mvaddstr(y + k, x + l, ".");
		}
	}
}

void draw_hallway(int points[][2], int length)
{
	if(length < 2)
	{
		return;
	}

	int x = points[0][0];
	int y = points[0][1];

	int direction_to_y_shift[] = {0, 1, 0, -1};
	int direction_to_x_shift[] = {1, 0, -1, 0};

	char direction_to_wall[][2] = {"|", "-", "|", "-"};
	char direction_to_ahead[][2] = {"-", "|", "-", "|"};
	for(int j = 1; j < length; j++)
	{
		int a = points[j][0];
		int b = points[j][1];

		int neg = a < 0 | b < 0;

		int direction = (neg << 1) + (a != 0);
		int y_shift = direction_to_y_shift[direction];
		int x_shift = direction_to_x_shift[direction];
		char * wall_string = direction_to_wall[direction];
		char * wall_ahead = direction_to_ahead[direction];

		int segment_length = a + b;
		int mask = -(segment_length < 0);
		segment_length += mask;
		segment_length ^= mask;

		for(int i = 0; i < segment_length; i++)
		{
			x += y_shift;
			y += x_shift;
			mvaddstr(y, x, ".");
			mvaddstr(y + y_shift, x + x_shift, wall_string);
			mvaddstr(y - y_shift, x - x_shift, wall_string);
			mvaddstr(y + x_shift, x + y_shift, wall_ahead);
		}
		if (j < length - 1)
		{
			mvaddstr(y + 1, x + 1, "+");
			mvaddstr(y - 1, x + 1, "+");
			mvaddstr(y + 1, x - 1, "+");
			mvaddstr(y - 1, x - 1, "+");
		}
	}
}

int main(void)
{
	initscr();
	cbreak();
	noecho();
	curs_set(0);

	keypad(stdscr, TRUE);

	player p = {10, 10};
	monster r = {20, 20, 'r', *giant_rat_update};
	for (int c = 0; c != 'q'; c = getch()) {
		player_input(&p, c);
		r.update(&r, p);

		clear();
		draw_room(5, 2, 7, 5);
		draw_hallway((int[][2]){{8,7}, {0,10}, {10,0}, {0,-5}, {-3, 0}}, 5);
		player_draw(&p);
		monster_draw(&r);
	}

	endwin();
}
