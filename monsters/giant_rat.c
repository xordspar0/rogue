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
	if (m->x + 1 < p.x) {
		if (floor_walkable(floor, m->x + 1, m->y)) {
			m->x++;
		}
	} else if (m->x - 1 > p.x) {
		if (floor_walkable(floor, m->x - 1, m->y)) {
			m->x--;
		}
	} else {
		if (m->y + 1 < p.y) {
			if (floor_walkable(floor, m->x, m->y + 1)) {
				m->y++;
			}
		} else if (m->y - 1 > p.y) {
			if (floor_walkable(floor, m->x, m->y - 1)) {
				m->y--;
			}
		}
	}
}
