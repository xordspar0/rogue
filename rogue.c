#include <curses.h>

#include "player.h"

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

int main(void) {
	initscr();
	cbreak();
	noecho();
	curs_set(0);

	keypad(stdscr, TRUE);


	struct player p = {10, 10};
	for (int c = 0; c != 'q'; c = getch()) {
		player_input(&p, c);

		clear();
		draw_room(5, 2, 7, 5);
		draw_hallway((int[][2]){{8,7}, {0,5}}, 2);
		player_draw(&p);
	}

	endwin();
}
