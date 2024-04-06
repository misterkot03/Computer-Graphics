#include "../headers/hero.h"
#include "../headers/level.h"
#define bool int
#define true 1
#define false 0
bool isAirborne = false;  // ���������� ��� ��������, ��������� �� �������� � �������
float gravity = -5.0f; // ���������, ����������� �� ��������� ��� �������
const float spriteHeight = 80.0f;  // ������ ������� ���������
const float spriteWidth = 80.0f;  // ������ ������� ���������
// ������� ��� �������� � ��������� ������������ ��������� � �������
void CheckCollisionAndAdjustPosition(Hero *hero, float tileWidth, float tileHeight, int levelWidth, int levelHeight) {
    // �������� ������ ������
    if (hero->x < 0) hero->x = 0;
    if (hero->x + spriteWidth > levelWidth * tileWidth) hero->x = levelWidth * tileWidth - spriteWidth;

    // �������� ������������ � ������� �����
    for (int x = 0; x < levelWidth; x++) {
        for (int y = 0; y < levelHeight; y++) {
            Tile tile = level[x][y];
            if (tile.type == TILE_TYPE_GROUND) {
                float tileX = x * tileWidth;
                float tileY = y * tileHeight;
                // ���������, ��������� �� ������ ����� ��������� ���� ������� ����� �����
                if (hero->y + spriteHeight >= tileY && hero->y < tileY + tileHeight &&
                    hero->x + spriteWidth > tileX && hero->x < tileX + tileWidth) {
                    hero->y = tileY - spriteHeight;  // �������� ��������� �� ����
                    hero->dy = 0;                    // ���������� ������������ ��������
                    isAirborne = false;              // �������� ������ �� � ������
                }
            }
        }
    }
}
void Hero_Init(Hero* hero, float x, float y) {
    hero->x = x;
    hero->y = y;
    hero->dx = 0.0f;
    hero->dy = 0.0f;
}

void UpdateHeroPosition(Hero *hero, float deltaTime) {
    // ��������� ����������, ���� �������� � �������
    if (isAirborne) {
        hero->dy += gravity * deltaTime;
    }

    // ��������� ������� ���������
    hero->x += hero->dx * deltaTime;
    hero->y += hero->dy * deltaTime;

    // ������������ ������������ � ������������ �������
    CheckCollisionAndAdjustPosition(hero, TILE_SIZE, TILE_SIZE, LEVEL_WIDTH, LEVEL_HEIGHT);
}

void Hero_Render(Hero *hero) {
    // ����� ����� ��� ���������� �����
}
