#include <windows.h>
#include <gl/gl.h>
#include <math.h>

float vert[] = {1,1,0, 1,-1,0, -1,-1,0, -1,1,0};
float xAlfa = 40;
float zAlfa = 0;
POINTFLOAT pos = {0, 0};
int n = 10;
static float coneTheta = 0.0f; // Начальное положение конуса
void drawCone(GLfloat radius, GLfloat height, GLint slices) {
    GLfloat angle;
    glBegin(GL_TRIANGLE_FAN);
        glVertex3f(0, 0, height); // Вершина конуса
        for (int i = 0; i <= slices; i++) {
            angle = 2.0 * M_PI * i / slices;
            glVertex3f(radius * cos(angle), radius * sin(angle), 0.0);
        }
    glEnd();

    // Основание конуса
    glColor3f(1,1,1);
    glBegin(GL_TRIANGLE_FAN);
        glVertex3f(0, 0, 0); // Центр основания
        for (int i = 0; i <= slices; i++) {
            angle = 2.0 * M_PI * i / slices;
            glVertex3f(radius * cos(angle), radius * sin(angle), 0.0);
        }
    glEnd();
}

void ShowWord() {
    updateSpotlightDirection(); // Обновляем направление света
    updateLightPositionAndDirection(); // Обновляем позицию света
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, vert);

    DWORD currentTime = GetTickCount();

    // Введем независимую переменную для вращения конуса

    coneTheta += 1.0f; // Скорость вращения конуса

    // Рассчитаем положение конуса с использованием новой переменной
    float radius = 5.0; // Радиус вращения конуса
    float cubeZ = 3.0;  // Высота куба
    float lightX = radius * sin(coneTheta * M_PI / 180);
    float lightZ = radius * cos(coneTheta * M_PI / 180) + cubeZ;

    // Расчет углов для вращения конуса
    float angleY = atan2(lightX, lightZ - cubeZ) * 180 / M_PI; // Горизонтальный угол вращения
    float hypotenuse = sqrt(lightX * lightX + (lightZ - cubeZ) * (lightZ - cubeZ));
    float angleX = atan2(1, hypotenuse) * 180 / M_PI; // Вертикальный угол наклона

    // Рисуем конус в новом положении
    glPushMatrix();
        glColor3f(1.0, 1.0, 0.0); // Желтый цвет для конуса
        glTranslatef(lightX, 0, lightZ); // Перемещаем конус в позицию света
        glRotatef(angleY, 0, 1, 0); // Вращение конуса вокруг оси Y
        glRotatef(-angleX, 1, 0, 0); // Наклон конуса для "просмотра" вниз к центру куба
        drawCone(0.5, 1, 16); // Радиус, высота и количество разбиений
    glPopMatrix();

    // Доска
    for (int i = -n; i < n; i++) {
        for (int j = -n; j < n; j++) {
            glPushMatrix();
            if ((i + j) % 2 == 0) {
                glColor3f(1.0f, 1.0f, 1.0f); // Цвет черных плиток
            } else {
                glColor3f(0.70, 0.98, 0.98); // Цвет белых плиток
                // Рисуем вращающийся треугольник на белой плитке
                glTranslatef(i * 2, j * 2, 0.1f); // Поднимаем треугольники немного выше плиток
                glRotatef(currentTime * 0.1f, 0, 0, 1); // Вращение треугольника
                glBegin(GL_TRIANGLES);
                    glColor3f(1.0f, 0.0f, 0.0f); glVertex3f(-0.5f, -0.5f, 0);
                    glColor3f(0.0f, 1.0f, 0.0f); glVertex3f(0.5f, -0.5f, 0);
                    glColor3f(0.0f, 0.0f, 1.0f); glVertex3f(0.0f, 0.5f, 0);
                glEnd();
                glPopMatrix();
                continue; // Пропустить обычный вызов glPopMatrix()
            }
            glTranslatef(i * 2, j * 2, 0);
            glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
            glPopMatrix();
        }
    }

    // ОСИ
    int axisLength = n * 2;
    int axisWidth = 10;
    glLineWidth(axisWidth);
    glBegin(GL_LINES);
        // Ось X, красная
        glColor3f(1, 0, 0);
        glVertex3f(0, 0, 0.1f); // Немного поднимаем оси над плитками
        glVertex3f(axisLength, 0, 0.1f); // Увеличенная длина

        // Ось Y, зеленая
        glColor3f(0, 1, 0);
        glVertex3f(0, 0, 0.1f);
        glVertex3f(0, axisLength, 0.1f); // Увеличенная длина

        // Ось Z, синяя
        glColor3f(0, 0, 1);
        glVertex3f(0, 0, 0.1f);
        glVertex3f(0, 0, axisLength); // Увеличенная длина
    glEnd();

    glDisableClientState(GL_VERTEX_ARRAY);
}

void updateSpotlightDirection() {
    float angleVerticalRadians = xAlfa * M_PI / 180.0;
    float angleHorizontalRadians = zAlfa * M_PI / 180.0;
    GLfloat spot_direction[] = {
        cos(angleVerticalRadians) * sin(angleHorizontalRadians),
        sin(angleVerticalRadians),
        cos(angleVerticalRadians) * cos(angleHorizontalRadians)
    };
    glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, spot_direction);
}

void updateLightPositionAndDirection() {
    float radius = 10.0;
    float lightX = radius * sin(coneTheta * M_PI / 180);
    float lightZ = radius * cos(coneTheta * M_PI / 180);

    float lightPos[] = {lightX, 0, lightZ, 1.0};
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);

    float lightDirection[] = {-lightX, 0, -lightZ};
    glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, lightDirection);
}


void MoveCamera()
{


     if (GetKeyState(VK_UP) < 0) {
        xAlfa += 1.0;

    }
    if (GetKeyState(VK_DOWN) < 0) {
        xAlfa -= 1.0;

    }
    if (GetKeyState(VK_LEFT) < 0) {
        zAlfa += 1.0;

    }
    if (GetKeyState(VK_RIGHT) < 0) {
        zAlfa -= 1.0;

    }

    float ungol = -zAlfa / 180 * M_PI;
    float speed = 0;
    if (GetKeyState('W') < 0) speed = 0.1;
    if (GetKeyState('S') < 0) speed = -0.1;
    if (GetKeyState('A') < 0) { speed = 0.1; ungol -= M_PI*0.5; }
    if (GetKeyState('D') < 0) { speed = 0.1; ungol += M_PI*0.5; }
    if (speed != 0)
    {
        pos.x += sin(ungol) * speed;
        pos.y += cos(ungol) * speed;
    }

    glRotatef(-xAlfa, 1, 0, 0);
    glRotatef(-zAlfa, 0, 0, 1);
    glTranslatef(-pos.x, -pos.y, -3);
}

float vertex[] = {-1,-1,1, 1,-1,1, 1,1,1, -1,1,1};
float normal[] = {-1,-1,1, 1,-1,1, 1,1,1, -1,1,1};/*векторы нормали*/
void Draw()
{
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);
        glVertexPointer(3, GL_FLOAT, 0, vertex);
        glNormalPointer(GL_FLOAT, 0, normal);
        glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);
}

void DrawCube()
{
    glPushMatrix();
    glColor3f(0.68, 0.85, 0.90);

        glScalef(1, 1, 1);/*размер куба*/
        glTranslatef(0, 0, 3);/*положение куба*/
        Draw();
        glRotatef(90, 1,0,0);
        Draw();
        glRotatef(90, 1,0,0);
        Draw();
        glRotatef(90, 1,0,0);
        Draw();
        glRotatef(90, 0,1,0);
        Draw();
        glRotatef(180, 0,1,0);
        Draw();
    glPopMatrix();
}



LRESULT CALLBACK WindowProc(HWND, UINT, WPARAM, LPARAM);
void EnableOpenGL(HWND hwnd, HDC*, HGLRC*);
void DisableOpenGL(HWND, HDC, HGLRC);


int WINAPI WinMain(HINSTANCE hInstance,
                   HINSTANCE hPrevInstance,
                   LPSTR lpCmdLine,
                   int nCmdShow)
{
    WNDCLASSEX wcex;
    HWND hwnd;
    HDC hDC;
    HGLRC hRC;
    MSG msg;
    BOOL bQuit = FALSE;
    float theta = 0.0f;

    /* register window class */
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_OWNDC;
    wcex.lpfnWndProc = WindowProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
    wcex.lpszMenuName = NULL;
    wcex.lpszClassName = "GLSample";
    wcex.hIconSm = LoadIcon(NULL, IDI_APPLICATION);;


    if (!RegisterClassEx(&wcex))
        return 0;

    /* create main window */
    hwnd = CreateWindowEx(0,
                          "GLSample",
                          "OpenGL Sample",
                          WS_OVERLAPPEDWINDOW,
                          CW_USEDEFAULT,
                          CW_USEDEFAULT,
                          1080,
                          1080,
                          NULL,
                          NULL,
                          hInstance,
                          NULL);

    ShowWindow(hwnd, nCmdShow);

    /* enable OpenGL for the window */
    EnableOpenGL(hwnd, &hDC, &hRC);

    glFrustum(-1,1, -1,1, 2,80);
    glEnable(GL_DEPTH_TEST); /*тест глубины*/


    glMatrixMode(GL_PROJECTION);/*матрица проекций*/
    glLoadIdentity();
    glFrustum(-0.1, 0.1, -0.1, 0.1, 0.2, 1000);
    glMatrixMode(GL_MODELVIEW);/*матрица моделей*/
    glLoadIdentity();

    glEnable(GL_LIGHTING);/*разрешение работы с светом*/
    glEnable(GL_LIGHT0);/*включение нулевого источника света*/
    glEnable(GL_COLOR_MATERIAL); /*включает освещение*/
    glEnable(GL_NORMALIZE);/*включает нормализацию вектора*/

    /* program main loop */
    while (!bQuit)
    {
        /* check for messages */
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            /* handle or dispatch messages */
            if (msg.message == WM_QUIT)
            {
                bQuit = TRUE;
            }
            else
            {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
        }
        else
        {
            /* OpenGL animation code goes here */

           glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);/*тест глубины*/

            glPushMatrix();
                MoveCamera();
                ShowWord();

                glPushMatrix();
                    glRotatef(0,0,0,1);
                    glRotatef(theta,0,1,0);
                    float position[] = {0,0,1,0};//вектор освещение сверху вниз
                    glLightfv(GL_LIGHT0, GL_POSITION, position);

                    glTranslatef(0,0,5);/*положение освещения для наглядности*/
                    glScalef(1,1,1);/*размер освещения*/
                    glColor3f(1,1,1);/*цвет освещения*/

                glPopMatrix();

                glColor3f(1,0,0);
                DrawCube();

            glPopMatrix();

            theta++;
            SwapBuffers(hDC);

            Sleep (1);
        }
    }

    /* shutdown OpenGL */
    DisableOpenGL(hwnd, hDC, hRC);

    /* destroy the window explicitly */
    DestroyWindow(hwnd);

    return msg.wParam;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
        case WM_CLOSE:
            PostQuitMessage(0);
        break;

        case WM_DESTROY:
            return 0;

        case WM_KEYDOWN:
        {
            switch (wParam)
            {
                case VK_ESCAPE:
                    PostQuitMessage(0);
                break;
            }
        }
        break;

        default:
            return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }

    return 0;
}

void EnableOpenGL(HWND hwnd, HDC* hDC, HGLRC* hRC)
{
    PIXELFORMATDESCRIPTOR pfd;

    int iFormat;

    /* get the device context (DC) */
    *hDC = GetDC(hwnd);

    /* set the pixel format for the DC */
    ZeroMemory(&pfd, sizeof(pfd));

    pfd.nSize = sizeof(pfd);
    pfd.nVersion = 1;
    pfd.dwFlags = PFD_DRAW_TO_WINDOW |
                  PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.cColorBits = 24;
    pfd.cDepthBits = 16;
    pfd.iLayerType = PFD_MAIN_PLANE;

    iFormat = ChoosePixelFormat(*hDC, &pfd);

    SetPixelFormat(*hDC, iFormat, &pfd);

    /* create and enable the render context (RC) */
    *hRC = wglCreateContext(*hDC);

    wglMakeCurrent(*hDC, *hRC);
}

void DisableOpenGL (HWND hwnd, HDC hDC, HGLRC hRC)
{
    wglMakeCurrent(NULL, NULL);
    wglDeleteContext(hRC);
    ReleaseDC(hwnd, hDC);
}

