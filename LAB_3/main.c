#include <windows.h>
#include <gl/gl.h>
#include "../LAB_3/stb-master/Menu.h"
#define STB_IMAGE_IMPLEMENTATION
#include "../LAB_3/stb-master/stb_image.h"
// ГЛОБАЛЬНЫЕ ПЕРЕМЕННЫЕ
int sprite1ButtonPressed = 0;
int sprite2ButtonPressed = 0;
int currentFrame = 0;          // Текущий кадр анимации
const int totalFrames = 8;     // Всего кадров в спрайт-листе
float frameWidth = 1.0f / 8.0f;  // Ширина одного кадра в текстурных координатах

LRESULT CALLBACK WindowProc(HWND, UINT, WPARAM, LPARAM);
void EnableOpenGL(HWND hwnd, HDC*, HGLRC*);
void DisableOpenGL(HWND, HDC, HGLRC);



GLuint LoadTexture(const char *filename)
{
    int width, height, cnt;
    unsigned char *image = stbi_load(filename, &width, &height, &cnt, 0);
    if (image == NULL) {
        printf("Error in loading the image: %s\n", stbi_failure_reason());
        exit(1);
    }
    printf("Loaded image '%s' with width: %d, height: %d, channels: %d\n", filename, width, height, cnt);
    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, cnt == 4 ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, image);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    stbi_image_free(image);
    return textureID;
}




GLuint textureSprite1, textureSprite2, textureBackground;

// Функция для обновления текущего кадра
void UpdateAnimationFrame() {
    currentFrame = (currentFrame + 1) % totalFrames; // Переход к следующему кадру
}

// Функция рендеринга анимации
void RenderSpriteAnimation(GLuint texture, float posX, float posY, float width, float height, float scale) {
    float frameWidth = 1.0f / 8; // Предполагается, что numFrames - это количество кадров в спрайт-листе
    float texLeft = currentFrame * frameWidth;
    float texRight = texLeft + frameWidth;

    // Рассчитываем размеры спрайта с учетом масштаба
    float scaledWidth = width * scale;
    float scaledHeight = height * scale;

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture);

    glColor3f(1,1,1);
    glBegin(GL_QUADS);
        glTexCoord2f(texLeft, 0.0f); glVertex2f(posX, posY);                               // Левый верхний угол
        glTexCoord2f(texRight, 0.0f); glVertex2f(posX + scaledWidth, posY);                 // Правый верхний угол
        glTexCoord2f(texRight, 1.0f); glVertex2f(posX + scaledWidth, posY + scaledHeight);  // Правый нижний угол
        glTexCoord2f(texLeft, 1.0f); glVertex2f(posX, posY + scaledHeight);                 // Левый нижний угол
    glEnd();

    glDisable(GL_TEXTURE_2D);
}

void Init()
{
    Menu_AddButton("sprite_1",10,10,100,30,2);
    Menu_AddButton("sprite_2",10,50,100,30,2);
    Menu_AddButton("Exit",10,90,100,30,2);

     textureSprite1 = LoadTexture("sprite1.png");
     textureSprite2 = LoadTexture("sprite2.png");
     textureBackground = LoadTexture("background.png");
}

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
                          0,
                          0,
                          1000,
                          1000,
                          NULL,
                          NULL,
                          hInstance,
                          NULL);

    ShowWindow(hwnd, nCmdShow);

    /* enable OpenGL for the window */
    EnableOpenGL(hwnd, &hDC, &hRC);
    RECT rct; //создание переменной с координатами прямоуголника


    glMatrixMode(GL_PROJECTION);
            glLoadIdentity();
            GetClientRect(hwnd, &rct);
            glOrtho(0, rct.right, rct.bottom, 0, 1, -1);
            glMatrixMode(GL_MODELVIEW);
            glLoadIdentity();


    Init();
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

            UpdateAnimationFrame();


            glClearColor(0.5f, 0.2f, 0.1f, 0.7f);
            glClear(GL_COLOR_BUFFER_BIT);

            glPushMatrix();

            Menu_ShowMenu();


            float centerX = rct.right / 2.0f;
            float posY = 150.0f;
            float spriteWidth = 800.0f; // ширина текстуры
            float spriteHeight = 80.0f; // высота текстуры


                    glEnable(GL_BLEND);
                    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);






                    if (sprite1ButtonPressed)
                    {
                        float sprite1PosX = centerX - (spriteWidth / 2);
                        float sprite1PosY = posY;



                        RenderSpriteAnimation(textureSprite1,sprite1PosX,sprite1PosY, 640.0f / 8.0f, 80.0f, 8.0f);
                    }

                    // Аналогично для Sprite_2
                    if (sprite2ButtonPressed)
                    {
                        float sprite2PosX = centerX - (spriteWidth / 2);
                        float sprite2PosY = posY;

                        RenderSpriteAnimation(textureSprite2, sprite2PosX,sprite2PosY, 640.0f / 8.0f, 80.0f, 8.0f);
                    }

            glDisable(GL_BLEND);



            glPopMatrix();

            SwapBuffers(hDC);

            theta += 1.0f;
            Sleep (80);
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


        case WM_MOUSEMOVE:
            Menu_MouseMove(LOWORD(lParam), HIWORD(lParam));
        break;

        case WM_LBUTTONDOWN:
{
                int buttonId = Menu_MouseDown();
                if (buttonId == 0)
                {
                    sprite1ButtonPressed = 1;
                    sprite2ButtonPressed = 0;
                    printf("Button Sprite_1 pressed. State: %d\n", sprite1ButtonPressed);
                }
                else if (buttonId == 1)
                {
                    sprite1ButtonPressed = 0;
                    sprite2ButtonPressed = 1;
                    printf("Button Sprite_2 pressed. State: %d\n", sprite2ButtonPressed);
                }
                else if (buttonId == 2)
                    PostQuitMessage(0);
            }
            break;

        case WM_LBUTTONUP:
            Menu_MouseUp();
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
