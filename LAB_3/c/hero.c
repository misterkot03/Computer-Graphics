#include "../headers/hero.h"
#include "../headers/level.h"
#define bool int
#define true 1
#define false 0
bool isAirborne = false;  // Переменная для проверки, находится ли персонаж в воздухе
float gravity = -5.0f; // Ускорение, действующее на персонажа при падении
const float spriteHeight = 80.0f;  // Высота спрайта персонажа
const float spriteWidth = 80.0f;  // Ширина спрайта персонажа
// Функция для проверки и обработки столкновений персонажа с тайлами
void CheckCollisionAndAdjustPosition(Hero *hero, float tileWidth, float tileHeight, int levelWidth, int levelHeight) {
    // Проверка границ уровня
    if (hero->x < 0) hero->x = 0;
    if (hero->x + spriteWidth > levelWidth * tileWidth) hero->x = levelWidth * tileWidth - spriteWidth;

    // Проверка столкновений с тайлами земли
    for (int x = 0; x < levelWidth; x++) {
        for (int y = 0; y < levelHeight; y++) {
            Tile tile = level[x][y];
            if (tile.type == TILE_TYPE_GROUND) {
                float tileX = x * tileWidth;
                float tileY = y * tileHeight;
                // Проверяем, находится ли нижняя часть персонажа ниже верхней части тайла
                if (hero->y + spriteHeight >= tileY && hero->y < tileY + tileHeight &&
                    hero->x + spriteWidth > tileX && hero->x < tileX + tileWidth) {
                    hero->y = tileY - spriteHeight;  // Помещаем персонажа на тайл
                    hero->dy = 0;                    // Сбрасываем вертикальную скорость
                    isAirborne = false;              // Персонаж больше не в прыжке
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
    // Применяем гравитацию, если персонаж в воздухе
    if (isAirborne) {
        hero->dy += gravity * deltaTime;
    }

    // Обновляем позицию персонажа
    hero->x += hero->dx * deltaTime;
    hero->y += hero->dy * deltaTime;

    // Обрабатываем столкновения и корректируем позицию
    CheckCollisionAndAdjustPosition(hero, TILE_SIZE, TILE_SIZE, LEVEL_WIDTH, LEVEL_HEIGHT);
}

void Hero_Render(Hero *hero) {
    // Здесь будет код рендеринга героя
}
