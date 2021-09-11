#include <curses.h>

#include "../player.h"
#include "giant_rat.h"

void monster_draw(monster *m)
{
  mvaddch(m->y, m->x, m->c);
}

void giant_rat_update(monster *m, player p)
{
  if (m->x < p.x) {
    m->x++;
  } else {
    m->x--;
  }
}
