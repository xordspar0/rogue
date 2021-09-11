typedef struct monster {
  int x;
  int y;
  char c;
  void (*update)(struct monster*, player);
} monster;

void monster_draw(monster*);
void giant_rat_update(monster*, player);
