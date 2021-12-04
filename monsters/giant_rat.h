#include "../player.h"

typedef struct monster {
  int x;
  int y;
  char c;
  void (*update)(Floor, struct monster*, player);
} monster;

void monster_draw(monster*);
void giant_rat_update(Floor, monster*, player);
