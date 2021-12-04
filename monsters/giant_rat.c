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
	if (m->x < p.x) {
		if (floor_walkable(floor, m->x + 1, m->y)) {
			m->x++;
		}
	} else {
		if (floor_walkable(floor, m->x - 1, m->y)) {
			m->x--;
		}
	}
}
