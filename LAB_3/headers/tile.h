#ifndef TILE_H_INCLUDED
#define TILE_H_INCLUDED
//#include "graphics.h"
#include <gl/gl.h>

// Тип тайла
typedef enum {
    TILE_TYPE_NONE,
    TILE_TYPE_BRICK,
    TILE_TYPE_GROUND,
    TILE_TYPE_BRIDGE
} TileType;

// Тайл уровня
typedef struct {
    TileType type;
    GLuint texture;
} Tile;
#endif // TILE_H_INCLUDED
