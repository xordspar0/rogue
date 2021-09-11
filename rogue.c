#include <curses.h>

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
	clear();

	draw_room(5, 2, 7, 5);

	mvaddstr(10, 10, "x <-- You are here");

	getch();
	endwin();
}
