#ifndef MOVEMENT_H_INCLUDED
#define MOVEMENT_H_INCLUDED
#include <windows.h>

// Структура для описания героя
typedef struct {
    float x, y;    // Позиция
    float dx, dy;  // Скорость
} Hero;

// Объявление глобальных переменных
extern Hero hero;
extern bool isJumping, isMoving, isAirborne;

// Объявление функций
void ProcessInput(WPARAM wParam, bool keyDown);
void UpdateHeroState(float deltaTime);


#endif // MOVEMENT_H_INCLUDED
