#ifndef HERO_H_INCLUDED
#define HERO_H_INCLUDED
#include "graphics.h"

typedef struct {
    float x, y;    // Позиция
    float dx, dy;  // Скорость
} Hero;
extern Hero hero;

void Hero_Init(Hero* hero, float x, float y);
void UpdateHeroPosition(Hero *hero, float deltaTime);
void Hero_Render(Hero *hero);


#endif // HERO_H_INCLUDED
