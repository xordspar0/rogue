#pragma once
/*
typedef struct monster_type{
	int ap;
	int dp;
	int hp;
	int sp;
	void (*update)(Floor, struct monster*, player);
} monster_type;
*/
typedef struct monster {
	int x;
	int y;
	char c;
	void (*update)(Floor, struct monster*, player);
} monster;
