#ifndef MOVEMENT_H_INCLUDED
#define MOVEMENT_H_INCLUDED
#include <windows.h>

// ��������� ��� �������� �����
typedef struct {
    float x, y;    // �������
    float dx, dy;  // ��������
} Hero;

// ���������� ���������� ����������
extern Hero hero;
extern bool isJumping, isMoving, isAirborne;

// ���������� �������
void ProcessInput(WPARAM wParam, bool keyDown);
void UpdateHeroState(float deltaTime);


#endif // MOVEMENT_H_INCLUDED
