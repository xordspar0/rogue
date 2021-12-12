#include <curses.h>

#include "player.h"

void player_input(Floor floor, player * p, int c)
{
	switch (c) {
	case 'w':
	case KEY_UP:
		if (floor_walkable(floor, p->x, p->y - 1)) {
			p->y--;
		}
		break;

	case 's':
	case KEY_DOWN:
		if (floor_walkable(floor, p->x, p->y + 1)) {
			p->y++;
		}
		break;

	case 'a':
	case KEY_LEFT:
		if (floor_walkable(floor, p->x - 1, p->y)) {
			p->x--;
		}
		break;

	case 'd':
	case KEY_RIGHT:
		if (floor_walkable(floor, p->x + 1, p->y)) {
			p->x++;
		}
		break;
	}
}

void player_draw(player * p)
{
	mvaddstr(p->y, p->x, "@");
}
