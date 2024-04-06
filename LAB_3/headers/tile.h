#ifndef TILE_H_INCLUDED
#define TILE_H_INCLUDED
//#include "graphics.h"
#include <gl/gl.h>

// ��� �����
typedef enum {
    TILE_TYPE_NONE,
    TILE_TYPE_BRICK,
    TILE_TYPE_GROUND,
    TILE_TYPE_BRIDGE
} TileType;

// ���� ������
typedef struct {
    TileType type;
    GLuint texture;
} Tile;
#endif // TILE_H_INCLUDED
