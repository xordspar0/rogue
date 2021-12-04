#pragma once

#include "floor.h"

typedef struct {
  int x;
  int y;
} player;

void player_input(Floor, player*, int);
void player_draw(player*);
