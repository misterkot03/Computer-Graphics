#ifndef LEVEL_H_INCLUDED
#define LEVEL_H_INCLUDED
#include "../headers/tile.h"

#define TILE_SIZE 32
#define LEVEL_WIDTH 31
#define LEVEL_HEIGHT 30
extern Tile level[LEVEL_WIDTH][LEVEL_HEIGHT];

void InitLevel();
void RenderLevel();
#endif // LEVEL_H_INCLUDED
