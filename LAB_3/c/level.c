#include "../headers/Level.h"
#define TILE_SIZE 32
const int groundTopRows = 2;  // ���������� ����� groundTexture1 ������
const int groundBottomRows = 2;  // ���������� ����� groundTexture2 �����
Tile level[LEVEL_WIDTH][LEVEL_HEIGHT];
void InitLevel() {
    // ��������� �������� ��� ������ �����
    GLuint brickTexture1 = LoadTexture("Brick_01.png");
    GLuint brickTexture2 = LoadTexture("Brick_02.png");
    GLuint groundTexture1 = LoadTexture("Ground_02.png");
    GLuint groundTexture2 = LoadTexture("Ground_06.png");
    GLuint bridgeTexture1 = LoadTexture("Bridge_01.png");

    // ���������� ������ �������
     for (int x = 0; x < LEVEL_WIDTH; x++) {
        for (int y = 0; y < LEVEL_HEIGHT; y++) {
            // ������ ������� ��� �������, �������� �� ���� ������
            if (y == LEVEL_HEIGHT - 1) {
                level[x][y].type = TILE_TYPE_BRICK;
                level[x][y].texture = (x % 2 == 0) ? brickTexture1 : brickTexture2;
            } else if (y == 0) {
                level[x][y].type = TILE_TYPE_GROUND;
                level[x][y].texture = groundTexture2; // ����� ����� groundTexture2
            } else if (y == 1) {
                level[x][y].type = TILE_TYPE_GROUND;
                level[x][y].texture = groundTexture1; // ����� ��� ������ ����� ����� groundTexture1
            } else {
                level[x][y].type = TILE_TYPE_NONE;
            }
        }
    }
}
// ������� ��������� ����� ������
void RenderLevel() {
    for (int x = 0; x < LEVEL_WIDTH; x++) {
        for (int y = 0; y < LEVEL_HEIGHT; y++) {
            // ������������, ��� �� ��������� ��������� � ������� ������ ����
            float posX = x * TILE_SIZE;
            float posY = (LEVEL_HEIGHT - y -1) * TILE_SIZE;
            RenderTile(&level[x][y], posX, posY, TILE_SIZE, TILE_SIZE);
        }
    }
}
