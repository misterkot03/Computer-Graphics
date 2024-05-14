#include <windows.h>
#include <gl/gl.h>
#include <math.h>

float vert[] = {1, 1, 0, 1, -1, 0, -1, -1, 0, -1, 1, 0};
float xAlfa = 40;
float zAlfa = 0;
POINTFLOAT pos = {0, 0};
int n = 10;
static float coneTheta = 0.0f; // Начальное положение конуса

void Init_Light() {
    glEnable(GL_LIGHTING);
    glShadeModel(GL_SMOOTH);
    GLfloat light_position[] = {0.0f, 0.0f, 5.0f, 1.0f};
    GLfloat light_spot_direction[] = {0.0, 0.0, -1.0, 1.0};
    GLfloat light_ambient[] = {0.3f, 0.3f, 0.3f, 1.0f};  // Увеличена интенсивность окружающего света
    GLfloat light_diffuse[] = {1.0f, 1.0f, 1.0f, 1.0f};  // Увеличена интенсивность рассеянного света
    GLfloat light_specular[] = {1.0f, 1.0f, 1.0f, 1.0f}; // Увеличена интенсивность зеркального света

    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 45); // Увеличен угол освещения
    glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, light_spot_direction);
    glLightf(GL_LIGHT0, GL_SPOT_EXPONENT, 2.0); // Уменьшена экспонента
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    glEnable(GL_LIGHT0);
}




void Init_Material() {
    glEnable(GL_COLOR_MATERIAL);
    glShadeModel(GL_SMOOTH);
    GLfloat material_ambient[] = {0.2f, 0.2f, 0.2f, 1.0f};
    GLfloat material_diffuse[] = {1.0f, 1.0f, 1.0f, 1.0f};
    GLfloat material_specular[] = {1.0f, 1.0f, 1.0f, 1.0f};
    GLfloat material_shininess[] = {50.0f};

    glMaterialfv(GL_FRONT, GL_AMBIENT, material_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, material_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, material_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, material_shininess);
}

void drawSphere(float radius, int slices, int stacks) {
    for (int i = 0; i <= stacks; ++i) {
        float V = i / (float) stacks;
        float phi = V * M_PI;

        glBegin(GL_TRIANGLE_STRIP);
        for (int j = 0; j <= slices; ++j) {
            float U = j / (float) slices;
            float theta = U * (M_PI * 2);

            float x = cos(theta) * sin(phi);
            float y = sin(theta) * sin(phi);
            float z = cos(phi);

            glNormal3f(x, y, z);
            glVertex3f(x * radius, y * radius, z * radius);

            x = cos(theta) * sin(phi + M_PI / stacks);
            y = sin(theta) * sin(phi + M_PI / stacks);
            z = cos(phi + M_PI / stacks);

            glNormal3f(x, y, z);
            glVertex3f(x * radius, y * radius, z * radius);
        }
        glEnd();
    }
}


void drawPrism(float baseRadius, float height, int sides) {
    float angleStep = 2 * M_PI / sides;
    glBegin(GL_TRIANGLE_FAN);
        glVertex3f(0, 0, height);
        for (int i = 0; i <= sides; i++) {
            float angle = i * angleStep;
            glVertex3f(baseRadius * cos(angle), baseRadius * sin(angle), height);
        }
    glEnd();

    glBegin(GL_TRIANGLE_FAN);
        glVertex3f(0, 0, 0);
        for (int i = 0; i <= sides; i++) {
            float angle = i * angleStep;
            glVertex3f(baseRadius * cos(angle), baseRadius * sin(angle), 0);
        }
    glEnd();

    glBegin(GL_QUAD_STRIP);
        for (int i = 0; i <= sides; i++) {
            float angle = i * angleStep;
            float x = baseRadius * cos(angle);
            float y = baseRadius * sin(angle);
            glVertex3f(x, y, 0);
            glVertex3f(x, y, height);
        }
    glEnd();
}





void ShowWord() {
    updateSpotlightDirection(); // Обновляем направление света
    updateLightPositionAndDirection(); // Обновляем позицию света
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, vert);

    DWORD currentTime = GetTickCount();

    coneTheta += 1.0f; // Скорость вращения сферы

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

    // Рисуем сферу в новом положении
    float radius = 5.0f; // Радиус вращения сферы
    float height = 7.0f; // Высота, на которой сфера будет вращаться (выше куба)

    float lightX = radius * cos(coneTheta * M_PI / 180.0f);
    float lightY = radius * sin(coneTheta * M_PI / 180.0f);
    float lightZ = height;

    glPushMatrix();
        glColor3f(1.0, 1.0, 0.0); // Желтый цвет для сферы
        glTranslatef(lightX, lightY, lightZ); // Перемещаем сферу в позицию света
        drawSphere(0.5f, 20, 20); // Радиус и сегменты сферы
    glPopMatrix();

    // Рисуем куб
    glPushMatrix();
        glTranslatef(0, 0, 3); // Положение куба
        DrawCube(); // Вызов функции для рисования куба с нормалями
    glPopMatrix();

    // Рисуем 10 призм с изменением прозрачности
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    for (int i = 0; i < 10; ++i) {
        float angle = i * 2.0 * M_PI / 10;
        float prismX = 5.0f * cos(angle); // Увеличенный радиус
        float prismY = 5.0f * sin(angle); // Увеличенный радиус
        float alpha = 1.0f - (i / 10.0f);

        glPushMatrix();
            glColor4f(0.5f, 0.5f, 1.0f, alpha); // Цвет и прозрачность призм
            glTranslatef(prismX, prismY, 0.0f); // Позиция призм на шахматной доске (с Z = 0.0f)
            drawPrism(1.0f, 2.0f, 11); // Увеличенный радиус основания призм и высота
        glPopMatrix();
    }
    glDisable(GL_BLEND);

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
    float radius = 5.0f; // Радиус вращения сферы
    float height = 7.0f; // Высота на которой сфера будет вращаться (выше куба)

    float lightX = radius * cos(coneTheta * M_PI / 180.0f);
    float lightY = radius * sin(coneTheta * M_PI / 180.0f);
    float lightZ = height;

    float lightPos[] = {lightX, lightY, lightZ, 1.0f};
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);

    float lightDirection[] = {-lightX, -lightY, -lightZ}; // Свет направлен вниз к центру доски
    glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, lightDirection);
}





void MoveCamera() {
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
    if (GetKeyState('A') < 0) { speed = 0.1; ungol -= M_PI * 0.5; }
    if (GetKeyState('D') < 0) { speed = 0.1; ungol += M_PI * 0.5; }
    if (speed != 0) {
        pos.x += sin(ungol) * speed;
        pos.y += cos(ungol) * speed;
    }

    glRotatef(-xAlfa, 1, 0, 0);
    glRotatef(-zAlfa, 0, 0, 1);
    glTranslatef(-pos.x, -pos.y, -3);
}

float vertex[] = {-1, -1, 1, 1, -1, 1, 1, 1, 1, -1, 1, 1};
float normal[] = {0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1}; /*векторы нормали*/
void Draw() {
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);
        glVertexPointer(3, GL_FLOAT, 0, vertex);
        glNormalPointer(GL_FLOAT, 0, normal);
        glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);
}

void DrawCube() {
    GLfloat vertices[] = {
        // Front face
        -1.0f, -1.0f,  1.0f, // 0
         1.0f, -1.0f,  1.0f, // 1
         1.0f,  1.0f,  1.0f, // 2
        -1.0f,  1.0f,  1.0f, // 3
        // Back face
        -1.0f, -1.0f, -1.0f, // 4
        -1.0f,  1.0f, -1.0f, // 5
         1.0f,  1.0f, -1.0f, // 6
         1.0f, -1.0f, -1.0f, // 7
        // Left face
        -1.0f, -1.0f, -1.0f, // 8
        -1.0f, -1.0f,  1.0f, // 9
        -1.0f,  1.0f,  1.0f, // 10
        -1.0f,  1.0f, -1.0f, // 11
        // Right face
         1.0f, -1.0f, -1.0f, // 12
         1.0f, -1.0f,  1.0f, // 13
         1.0f,  1.0f,  1.0f, // 14
         1.0f,  1.0f, -1.0f, // 15
        // Top face
        -1.0f,  1.0f, -1.0f, // 16
        -1.0f,  1.0f,  1.0f, // 17
         1.0f,  1.0f,  1.0f, // 18
         1.0f,  1.0f, -1.0f, // 19
        // Bottom face
        -1.0f, -1.0f, -1.0f, // 20
         1.0f, -1.0f, -1.0f, // 21
         1.0f, -1.0f,  1.0f, // 22
        -1.0f, -1.0f,  1.0f  // 23
    };

    GLuint indices[] = {
        0,  1,  2,  2,  3,  0,   // Front face
        4,  5,  6,  6,  7,  4,   // Back face
        8,  9,  10, 10, 11, 8,   // Left face
        12, 13, 14, 14, 15, 12,  // Right face
        16, 17, 18, 18, 19, 16,  // Top face
        20, 21, 22, 22, 23, 20   // Bottom face
    };

    GLfloat normals[] = {
        // Front face
        0.0f,  0.0f,  1.0f,
        0.0f,  0.0f,  1.0f,
        0.0f,  0.0f,  1.0f,
        0.0f,  0.0f,  1.0f,
        // Back face
        0.0f,  0.0f, -1.0f,
        0.0f,  0.0f, -1.0f,
        0.0f,  0.0f, -1.0f,
        0.0f,  0.0f, -1.0f,
        // Left face
        -1.0f,  0.0f,  0.0f,
        -1.0f,  0.0f,  0.0f,
        -1.0f,  0.0f,  0.0f,
        -1.0f,  0.0f,  0.0f,
        // Right face
        1.0f,  0.0f,  0.0f,
        1.0f,  0.0f,  0.0f,
        1.0f,  0.0f,  0.0f,
        1.0f,  0.0f,  0.0f,
        // Top face
        0.0f,  1.0f,  0.0f,
        0.0f,  1.0f,  0.0f,
        0.0f,  1.0f,  0.0f,
        0.0f,  1.0f,  0.0f,
        // Bottom face
        0.0f, -1.0f,  0.0f,
        0.0f, -1.0f,  0.0f,
        0.0f, -1.0f,  0.0f,
        0.0f, -1.0f,  0.0f
    };

    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, vertices);

    glEnableClientState(GL_NORMAL_ARRAY);
    glNormalPointer(GL_FLOAT, 0, normals);

    glColor3f(0.68f, 0.85f, 0.90f); // Бирюзовый цвет
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, indices);

    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);
}




LRESULT CALLBACK WindowProc(HWND, UINT, WPARAM, LPARAM);
void EnableOpenGL(HWND hwnd, HDC*, HGLRC*);
void DisableOpenGL(HWND, HDC, HGLRC);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
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
    hwnd = CreateWindowEx(0, "GLSample", "OpenGL Sample", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 1080, 1080, NULL, NULL, hInstance, NULL);

    ShowWindow(hwnd, nCmdShow);

    /* enable OpenGL for the window */
    EnableOpenGL(hwnd, &hDC, &hRC);

    glFrustum(-1, 1, -1, 1, 2, 80);
    glEnable(GL_DEPTH_TEST); /*тест глубины*/

    glMatrixMode(GL_PROJECTION); /*матрица проекций*/
    glLoadIdentity();
    glFrustum(-0.1, 0.1, -0.1, 0.1, 0.2, 1000);
    glMatrixMode(GL_MODELVIEW); /*матрица моделей*/
    glLoadIdentity();

    Init_Light();
    Init_Material();

    glEnable(GL_LIGHTING); /*разрешение работы с светом*/
    glEnable(GL_LIGHT0); /*включение нулевого источника света*/
    glEnable(GL_COLOR_MATERIAL); /*включает освещение*/
    glEnable(GL_NORMALIZE); /*включает нормализацию вектора*/

    /* program main loop */
    while (!bQuit) {
        /* check for messages */
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
            /* handle or dispatch messages */
            if (msg.message == WM_QUIT) {
                bQuit = TRUE;
            } else {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
        } else {
            /* OpenGL animation code goes here */
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

            glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); /*тест глубины*/

            glPushMatrix();
                MoveCamera();
                ShowWord();

                glPushMatrix();
                    glRotatef(0, 0, 0, 1);
                    glRotatef(theta, 0, 1, 0);
                    float position[] = {0, 0, 1, 0}; // вектор освещение сверху вниз
                    glLightfv(GL_LIGHT0, GL_POSITION, position);

                    glTranslatef(0, 0, 5); /*положение освещения для наглядности*/
                    glScalef(1, 1, 1); /*размер освещения*/
                    glColor3f(1, 1, 1); /*цвет освещения*/
                glPopMatrix();

                glColor3f(1, 0, 0);
                //DrawCube();
            glPopMatrix();

            theta++;
            SwapBuffers(hDC);

            Sleep(1);
        }
    }

    /* shutdown OpenGL */
    DisableOpenGL(hwnd, hDC, hRC);

    /* destroy the window explicitly */
    DestroyWindow(hwnd);

    return msg.wParam;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
        case WM_CLOSE:
            PostQuitMessage(0);
            break;
        case WM_DESTROY:
            return 0;
        case WM_KEYDOWN:
            switch (wParam) {
                case VK_ESCAPE:
                    PostQuitMessage(0);
                    break;
            }
            break;
        default:
            return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
    return 0;
}

void EnableOpenGL(HWND hwnd, HDC* hDC, HGLRC* hRC) {
    PIXELFORMATDESCRIPTOR pfd;
    int iFormat;

    /* get the device context (DC) */
    *hDC = GetDC(hwnd);

    /* set the pixel format for the DC */
    ZeroMemory(&pfd, sizeof(pfd));
    pfd.nSize = sizeof(pfd);
    pfd.nVersion = 1;
    pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
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

void DisableOpenGL(HWND hwnd, HDC hDC, HGLRC hRC) {
    wglMakeCurrent(NULL, NULL);
    wglDeleteContext(hRC);
    ReleaseDC(hwnd, hDC);
}
