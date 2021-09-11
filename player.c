#include <curses.h>

#include "player.h"

void player_input(player *p, int c)
{
		switch (c) {
			case KEY_UP:
			p->y -= 1;
			break;

			case KEY_DOWN:
			p->y += 1;
			break;

			case KEY_LEFT:
			p->x -= 1;
			break;

			case KEY_RIGHT:
			p->x += 1;
			break;
		}
}

void player_draw(player *p)
{
		mvaddstr(p->y, p->x, "@");
}
