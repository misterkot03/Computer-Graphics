#ifndef GRAPHICS_H_INCLUDED
#define GRAPHICS_H_INCLUDED
#include <gl/gl.h>
#include "../headers/tile.h"

void RenderTile(Tile *tile, float x, float y, float width, float height);
void RenderLevel();
void RenderSpriteAnimation(GLuint texture, float posX, float posY, float width, float height, float scale);
GLuint LoadTexture(const char *filename);


#endif // GRAPHICS_H_INCLUDED
