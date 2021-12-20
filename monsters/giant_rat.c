#include <curses.h>

#include "../floor.h"
#include "../player.h"
#include "giant_rat.h"

void monster_draw(monster * m)
{
	mvaddch(m->y, m->x, m->c);
}

void giant_rat_update(Floor floor, monster * m, player p)
{
	int dx = m->x - p.x;
	int dy = m->y - p.y;
	int d = dx * dx + dy * dy;
	if (d > 1) {
		if ((m->x < p.x) && floor_walkable(floor, m->x + 1, m->y)) {
			m->x++;
		} else if ((m->x > p.x)
			   && floor_walkable(floor, m->x - 1, m->y)) {
			m->x--;
		} else if ((m->y < p.y)
			   && floor_walkable(floor, m->x, m->y + 1)) {
			m->y++;
		} else if ((m->y > p.y)
			   && floor_walkable(floor, m->x, m->y - 1)) {
			m->y--;
		}
	}
}
