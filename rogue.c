#include <curses.h>

int main(void) {
	initscr();
	cbreak();
	noecho();
	clear();

	mvaddstr(10, 10, "x <-- You are here");

	getch();
	endwin();
}
