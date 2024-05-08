#include <windows.h>
#include <gl/gl.h>
#include "stb-master/Menu.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb-master/stb_image.h"
#define bool int
#define true 1
#define false 0
#include <math.h>
#define H 27 // ������ �����
#define W 48 // ������ �����
#define TILE_SIZE 40.0f
#include <float.h>
int sign(float value);
bool isSolidTileAt(float x, float y);
bool isSolidTile(char tile);
// ���������� ����������
int sprite1ButtonPressed = 0;
int sprite2ButtonPressed = 0;
int currentFrame = 0;          // ������� ���� ��������
const int totalFrames = 8;     // ����� ������ � ������-�����
float frameWidth = 1.0f / 8.0f;  // ������ ������ ����� � ���������� �����������
int isMoving = 0;
float jumpSpeed = 47.0f; // ��������� �������� ������
float gravity = -5.0f; // ���������, ����������� �� ��������� ��� �������
float verticalVelocity = 0.0f; // ������������ �������� ���������
float maxYVelocity = 47.0f;
bool isJumping = false; // ��������� �� �������� � ������
float groundLevel = 0.0f; // ������� "�����", ���� �������� �������� �� ����� ����������
bool isAirborne = false;  // ���������� ��� ��������, ��������� �� �������� � �������
int jumpFrame = 0; // ������� ���� �������� ������
int jumpAnimationPlaying = 0; // ��������� ������������ �������� ������
const int jumpFramesCount = 80;
float jumpAnimationDuration = 0.0f;
 float totalJumpDuration;
 const float blockSize = 128.0f;
 //const int H = 30; // ������ �����
//const int W = 31; // ������ �����
GLuint textureSprite1, textureSprite2, textureSprite3, textureBackground, groundTile1, groundTile2, brickTile1, brickTile2;
char TileMap[H][W] = {
    "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX",
    "X                                              X",
    "X                                              X",
    "X                                              X",
    "X                                              X",
    "X                                              X",
    "XHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHH            X",
    "X                                              X",
    "X                                              X",
    "X                                              X",
    "X                                              X",
    "X                  HHHHHHHHHHHHHHHHHHHHHHHHHHHHX",
    "X                                              X",
    "X                                              X",
    "X                                              X",
    "X                                              X",
    "X                                              X",
    "X    HHHHHHHHHHHH                              X",
    "X                         HHHHHHHHH            X",
    "X                                              X",
    "X                                              X",
    "X                                              X",
    "X                                              X",
    "XBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBX",
    "XGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGX",
    "XGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGX",
    "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX",
};
typedef struct {
    float x, y;          // �������
    float dx, dy;        // ��������
    bool isAirborne;     // ��������� �� � �������
    bool isMoving;       // ��������� ��
    float width, height; // ������� �����
    bool jumpPeakReached;// ��������� �� ��� ������
    bool facingLeft;     // ������� �� �����
} Hero;
Hero hero = { .x = 0.0f, .y = 0.0f, .dx = 0.0f, .dy = 0.0f, .isAirborne = true, .isMoving = false, .width = 80.0f, .height = 80.0f };
//�������

void DrawMap() {
    const float blockSize = 40.0f; // ������ ������� ����� �� ������
    for (int i = 0; i < H; ++i) {
        for (int j = 0; j < W; ++j) {
            char tile = TileMap[i][j];
            GLuint textureID = 0;

            // ���������� �������� ��� �������� �����
            switch(tile) {
                case 'G': // �����
                    textureID =  groundTile2; // ����������� �������
                    break;
                case 'B': // �����
                    textureID =  groundTile1; // ����������� �������
                    break;
                case 'H': // �����
                    textureID =  groundTile1; // ����������� �������
                    break;
                case 'X': // ������
                    textureID = (i + j) % 2 == 0 ? brickTile1 : brickTile2; // ����������� �������
                    break;
                // �������� �������������� ������� ��� ������ ����� ������
                default:
                    continue; // ���������� ������ ������������ ��� ����������� �������
            }

            // ��������� �����
            glEnable(GL_TEXTURE_2D);
            glBindTexture(GL_TEXTURE_2D, textureID);
            glBegin(GL_QUADS);
                glTexCoord2f(0.0f, 0.0f); glVertex2f(j * blockSize, i * blockSize);
                glTexCoord2f(1.0f, 0.0f); glVertex2f(j * blockSize + blockSize, i * blockSize);
                glTexCoord2f(1.0f, 1.0f); glVertex2f(j * blockSize + blockSize, i * blockSize + blockSize);
                glTexCoord2f(0.0f, 1.0f); glVertex2f(j * blockSize, i * blockSize + blockSize);
            glEnd();
            glDisable(GL_TEXTURE_2D);
        }
    }
}


LRESULT CALLBACK WindowProc(HWND, UINT, WPARAM, LPARAM);
void EnableOpenGL(HWND hwnd, HDC*, HGLRC*);
void DisableOpenGL(HWND, HDC, HGLRC);



GLuint LoadTexture(const char *filename)
{
    int width, height, cnt;
    unsigned char *image = stbi_load(filename, &width, &height, &cnt, 0);
    if (image == NULL) {-+
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


void DrawBackground(GLuint texture)
{
    // ���������� ������ ������������� �������� ����
    static float vertices[] = {0.0f, 0.0f,  1920.0f, 0.0f,  1920.0f, 1080.0f,  0.0f, 1080.0f};

    // ���������� �������� �� 0 �� 1 ��� ������� ��������
    static float TexCord[] = {0, 0,  1, 0,  1, 1,  0, 1};

    glClearColor(0, 0, 0, 0); // ������ ���� ������� ������, ���� ����� ����������

    glEnable(GL_TEXTURE_2D); // �������� 2D ���������������
    glBindTexture(GL_TEXTURE_2D, texture); // ����������� ��������

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);

    glVertexPointer(2, GL_FLOAT, 0, vertices); // ��������� ������ ������
    glTexCoordPointer(2, GL_FLOAT, 0, TexCord); // ��������� ������ ��������� ��������

    glDrawArrays(GL_QUADS, 0, 4); // ������ ��������������� �� 4 ������

    glDisableClientState(GL_VERTEX_ARRAY); // ��������� ������ ������
    glDisableClientState(GL_TEXTURE_COORD_ARRAY); // ��������� ������ ��������� ��������

    glDisable(GL_TEXTURE_2D); // ��������� 2D ���������������
}



// ������� ��� ���������� �������� �����
void UpdateAnimationFrame() {

        currentFrame = (currentFrame + 1) % totalFrames; // ������� � ���������� �����

}
void DrawCollisionBox(Hero *hero) {
    glColor3f(1.0f, 0.0f, 0.0f); // ��������� �������� ����� ��� �������������� ��������
    glDisable(GL_TEXTURE_2D); // ���������� ��������������� ��� ��������� ������� ����

    glBegin(GL_LINE_LOOP); // �������� �������� �����
        glVertex2f(hero->x, hero->y);
        glVertex2f(hero->x + hero->width, hero->y);
        glVertex2f(hero->x + hero->width, hero->y + hero->height);
        glVertex2f(hero->x, hero->y + hero->height);
    glEnd();

    glEnable(GL_TEXTURE_2D); // ��������� ��������������� ������� ��� ��������� ����� �����
    glColor3f(1.0f, 1.0f, 1.0f); // ����������� ����� � ������ ��� ����������� ��������� �������
}

// ������� ���������� ��������
void RenderSpriteAnimation(GLuint texture, float posX, float posY, float width, float height, float scale, int currentFrame) {
     float frameWidth = 1.0f / 8.0f;
    float texLeft = currentFrame * frameWidth;
    float texRight = texLeft + frameWidth;

    // �������� ������ �� �����������, ���� ����� ������� �����
    if (hero.facingLeft) {
        float temp = texLeft;
        texLeft = texRight;
        texRight = temp;
    }

    // ������������ ������� ������� � ������ ��������
    float scaledWidth = width * scale;
    float scaledHeight = height * scale;
    DrawCollisionBox(&hero);

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture);

    glColor3f(1,1,1);
    glBegin(GL_QUADS);
        glTexCoord2f(texLeft, 0.0f); glVertex2f(posX, posY);                               // ����� ������� ����
        glTexCoord2f(texRight, 0.0f); glVertex2f(posX + scaledWidth, posY);                 // ������ ������� ����
        glTexCoord2f(texRight, 1.0f); glVertex2f(posX + scaledWidth, posY + scaledHeight);  // ������ ������ ����
        glTexCoord2f(texLeft, 1.0f); glVertex2f(posX, posY + scaledHeight);                 // ����� ������ ����
    glEnd();

    glDisable(GL_TEXTURE_2D);
}




bool isSolidTileAt(float x, float y) {
    // ����������� ���������� � ������� ������� TileMap
    int tileX = (int)(x / TILE_SIZE);
    int tileY = (int)(y / TILE_SIZE);

    // ���������, �� ������� �� ������� �� ������� �������
    if (tileX < 0 || tileX >= W || tileY < 0 || tileY >= H) {
        return false; // ���������� false, ���� ���������� ��� ��������� �����
    }

    // �������� ������ ����� �� ��������
    char tile = TileMap[tileY][tileX];

    // ���������� true, ���� ���� �������� ������������
    return tile == 'X' || tile == 'B' || tile == 'H' || tile == 'G';
}

void UpdateGroundLevelForHero(Hero* hero) {
    float nearestGround = FLT_MAX;
    bool groundFound = false;

    // �������� �������� � ������� ����� �� X � ��������� ������ ����� ����� ��� ���
    int tileXStart = (int)(hero->x / TILE_SIZE);
    int tileXEnd = (int)((hero->x + hero->width) / TILE_SIZE);

    // ���� ��������� ������������ ���� ��� ������
    for (int x = tileXStart; x <= tileXEnd; x++) {
        for (int y = (int)(hero->y / TILE_SIZE) + 1; y < H; y++) {
            if (isSolidTileAt(x * TILE_SIZE, y * TILE_SIZE)) {
                float groundY = y * TILE_SIZE - hero->height;
                if (groundY < nearestGround) {
                    nearestGround = groundY;
                    groundFound = true;
                }
                break; // ��������� � ���������� �������, ��� ������ ������ �����
            }
        }
    }

    if (groundFound) {
        groundLevel = nearestGround;
    } else {
        // ���� ��� ������ ��� �����, �� ����� ���������� groundLevel
        // � ��������, ������� ���������� "�������" ��� ������� ��� �� ��������� �������
        groundLevel = FLT_MAX; // �������� ���������� ����� ��� ������
    }
}
bool CheckHorizontalCollision(float newX, Hero *hero, bool* isWallHitX) {
    *isWallHitX = false;
    bool collision = false;

    int leftTile = floor(newX / TILE_SIZE);
    int rightTile = ceil((newX + hero->width) / TILE_SIZE) - 1;

    for (int y = floor(hero->y / TILE_SIZE); y <= ceil((hero->y + hero->height) / TILE_SIZE) - 1; y++) {
        for (int x = leftTile; x <= rightTile; x++) {
            if (x < 0 || x >= W || y < 0 || y >= H) continue;
            char tile = TileMap[y][x];
            if (tile == 'X' || tile == 'B' || tile == 'H' || tile == 'G') {
                collision = true;
                if (x == leftTile || x == rightTile) {
                    *isWallHitX = true;
                    break;
                }
            }
        }
        if (*isWallHitX) break; // ��������� ����� �� ����� ��� ����������� ������������
    }
    return collision;
}
bool CheckVerticalCollision(float newY, Hero *hero, bool* isWallHitY) {
    *isWallHitY = false;
    bool collision = false;

    int topTile = floor(newY / TILE_SIZE);
    int bottomTile = ceil((newY + hero->height) / TILE_SIZE) - 1;

    for (int x = floor(hero->x / TILE_SIZE); x <= ceil((hero->x + hero->width) / TILE_SIZE) - 1; x++) {
        for (int y = topTile; y <= bottomTile; y++) {
            if (x < 0 || x >= W || y < 0 || y >= H) continue;
            char tile = TileMap[y][x];
            if (tile == 'X' || tile == 'B' || tile == 'H' || tile == 'G') {
                collision = true;
                if (y == topTile || y == bottomTile) {
                    *isWallHitY = true;
                    break;
                }
            }
        }
        if (*isWallHitY) break; // ��������� ����� �� ����� ��� ����������� ������������
    }
    return collision;
}


void UpdateHeroPositionAndCollisions(Hero *hero, float deltaTime) {
    // ��������� ������� ����� ��� �����
    UpdateGroundLevelForHero(hero);

    // �������������� ����� ������� ����� �� ��� X
    float potentialNewX = hero->x + hero->dx * deltaTime;
    bool isWallHitX = false;

    // �������� ������������ �� ��� X
    if (!CheckHorizontalCollision(potentialNewX, hero, &isWallHitX)) {
        hero->x = potentialNewX;
    } else {
        // ��� ������������ ������ ��� ����� ������������ �������
        if (hero->dx > 0) { // �������� ������
            hero->x = floor((potentialNewX + hero->width) / TILE_SIZE) * TILE_SIZE - hero->width - 0.01; // ������� ���������� �� �����
        } else if (hero->dx < 0) { // �������� �����
            hero->x = ceil(potentialNewX / TILE_SIZE) * TILE_SIZE + 0.01; // ������� ���������� �� �����
        }
        hero->dx = 0; // ������������� �������� �� X
    }

    // ���������� ���������� � ���������� �� Y
    hero->dy -= gravity * deltaTime;
    float potentialNewY = hero->y + hero->dy * deltaTime;
    bool isWallHitY = false;

    // �������� ������������ �� ��� Y
    if (!CheckVerticalCollision(potentialNewY, hero, &isWallHitY)) {
        hero->y = potentialNewY;
        hero->isAirborne = true; // ����� ��������� � �������
    } else {
        // ��� ������������ ������ ��� �����
        if (hero->dy > 0) { // ���� �������� �����
            hero->y = ceil((hero->y + hero->height) / TILE_SIZE) * TILE_SIZE - hero->height - 0.01;
        } else { // ���� �������� ����
            hero->y = floor(potentialNewY / TILE_SIZE) * TILE_SIZE;
            hero->isAirborne = false; // ����� ����� �� �����
        }
        hero->dy = 0;
    }


    // ����������� ������������ ��������
    if (hero->dy > maxYVelocity) {
        hero->dy = maxYVelocity;
    } else if (hero->dy < -maxYVelocity) {
        hero->dy = -maxYVelocity;
    }
}



void Init(HWND hwnd)
{
    Menu_AddButton("sprite_1",10,10,100,30,2);
    Menu_AddButton("sprite_2",10,50,100,30,2);
    Menu_AddButton("Exit",10,90,100,30,2);

     textureSprite1 = LoadTexture("sprite1.png");
     textureSprite2 = LoadTexture("sprite2.png");
     textureSprite3 = LoadTexture("IDLE.png");
     textureBackground = LoadTexture("Background.png");
     groundTile1 = LoadTexture("Ground_02.png");
     groundTile2 = LoadTexture("Ground_06.png");
     brickTile1 = LoadTexture("Brick_01.png");
     brickTile2 = LoadTexture("Brick_02.png");
     RECT rct;
     GetClientRect(hwnd, &rct);
     groundLevel = rct.bottom - 80;
     hero.facingLeft = false;

     hero.dy = 0.0f;


     // ������������� ������� �����
     //isAirborne = true;
     hero.x = 200.0f;  // ��������� ������� �� X
     hero.y = groundLevel-100;  // ��������� ������� �� Y
     hero.isAirborne = false;
}
void UpdateJumpAnimationFrame(float deltaTime) {
    if (jumpAnimationPlaying) {
        jumpAnimationDuration += deltaTime;
        // �����������, ��� �������� �������������� ���������� �� ����� ������
        float progress = jumpAnimationDuration / totalJumpDuration;
        jumpFrame = (int)(progress * jumpFramesCount);

        // ��������� ��������, ���� �������� ����� ��� ���� ����� �����������
        if (jumpFrame >= jumpFramesCount || !isAirborne) {
            jumpFrame = 0; // ����� � ������� ����� ��� ���������� ������
            jumpAnimationPlaying = 0;
            jumpAnimationDuration = 0.0f; // ����� ������������ ��������
        }
    }
}
void UpdateGeneralAnimationFrame() {

        currentFrame = (currentFrame + 1) % totalFrames; // ����������� ���������� �����

}

int WINAPI WinMain(HINSTANCE hInstance,
                   HINSTANCE hPrevInstance,
                   LPSTR lpCmdLine,
                   int nCmdShow)
{
    glViewport(0, 0, 1000, 1000);
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
                          1920,
                          1080,
                          NULL,
                          NULL,
                          hInstance,
                          NULL);

    ShowWindow(hwnd, nCmdShow);

    /* enable OpenGL for the window */
    EnableOpenGL(hwnd, &hDC, &hRC);
    RECT rct; //�������� ���������� � ������������ �������������


    glMatrixMode(GL_PROJECTION);
            glLoadIdentity();
            GetClientRect(hwnd, &rct);
            glOrtho(0, rct.right, rct.bottom, 0, 1, -1);
            glMatrixMode(GL_MODELVIEW);
            glLoadIdentity();


    Init(hwnd);
    DWORD lastUpdateTime = GetTickCount();
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
            DWORD currentTime = GetTickCount();
            float deltaTime = (currentTime - lastUpdateTime) / 1000.0f; // ����� � ��������
            lastUpdateTime = currentTime;
            //UpdateHero(deltaTime);






            glClearColor(0.5f, 0.2f, 0.1f, 0.7f);
            glClear(GL_COLOR_BUFFER_BIT);
            DrawBackground(textureBackground);
            DrawMap();
            float centerX = rct.right / 2.0f;
            float posY = 150.0f;
            float spriteWidth = 80.0f; // ������ ��������
            float spriteHeight = 80.0f; // ������ ��������


                    glEnable(GL_BLEND);
                    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);



                    float spriteAspectRatio = (float)80 / (float)80;
                    float renderedSpriteWidth = spriteHeight * spriteAspectRatio;
                    UpdateHeroPositionAndCollisions(&hero, deltaTime);






                    if(!isAirborne && !isMoving ){
                            glPushMatrix();

                             UpdateGeneralAnimationFrame();

                    RenderSpriteAnimation(textureSprite3, hero.x, hero.y, renderedSpriteWidth, spriteHeight, 1.0f, currentFrame);
                    glPopMatrix();
                    }
                    if (isMoving) {
                            glPushMatrix();


                     UpdateGeneralAnimationFrame();
                    hero.x += hero.dx;
                    hero.y += hero.dy;
                     if (hero.x < 0) hero.x = 0;
                    if (hero.x > rct.right - spriteWidth) hero.x = rct.right - spriteWidth;
                    RenderSpriteAnimation(textureSprite1, hero.x, hero.y, renderedSpriteWidth, spriteHeight, 1.0f, currentFrame);
                    glPopMatrix();

                    } if (isAirborne) {
                    glPushMatrix();
                    if (!jumpAnimationPlaying) {
                        jumpFrame = 0; // �������� �������� � ������� �����
                        jumpAnimationPlaying = 1; // ���������� �������� ������
                    }
                    UpdateGeneralAnimationFrame();


                    RenderSpriteAnimation(textureSprite2, hero.x, hero.y, renderedSpriteWidth, spriteHeight, 1.0f, currentFrame);

                    hero.y -= verticalVelocity; // ��������, ������ ��� �������� �����
                    verticalVelocity += gravity; // ��������� ����������

                    if (hero.y >= groundLevel) {
                        hero.y = groundLevel;
                        isAirborne = false;
                        verticalVelocity = 0;
                        jumpAnimationPlaying = 0; // ������������� �������� ������
                    }
                    glPopMatrix();
                }
                //Collision(&hero);

    glDisable(GL_BLEND);




            //Menu_ShowMenu();






/*
                    if (sprite1ButtonPressed)
                    {
                        float sprite1PosX = centerX - (spriteWidth / 2);
                        float sprite1PosY = posY;



                        RenderSpriteAnimation(textureSprite1,sprite1PosX,sprite1PosY, 640.0f / 8.0f, 80.0f, 8.0f);
                    }

                    // ���������� ��� Sprite_2
                    if (sprite2ButtonPressed)
                    {
                        float sprite2PosX = centerX - (spriteWidth / 2);
                        float sprite2PosY = posY;

                        RenderSpriteAnimation(textureSprite2, sprite2PosX,sprite2PosY, 640.0f / 8.0f, 80.0f, 8.0f);
                    }
*/
            glDisable(GL_BLEND);





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

    switch(wParam) {
        case VK_LEFT: // ���� ������ ������� �����
            hero.dx = -15.0f; // ������������� �������� �������� �����
            isMoving = true;
            hero.facingLeft = true; // ����� ������� �����
            break;
        case VK_RIGHT: // ���� ������ ������� ������
            hero.dx = 15.0f; // ������������� �������� �������� ������
            hero.facingLeft = false; // ����� ������� ������
            isMoving = true;
            break;
        case VK_UP: // ������� �����
              if (!isAirborne) {
                    isAirborne = true;
        verticalVelocity = jumpSpeed; // ������

    }
            break;

    }
    break;

    case WM_KEYUP:
    if (wParam == VK_LEFT || wParam == VK_RIGHT) {
        hero.dx = 0.0f;
        isMoving = 0; // ���������, ��������, ��� ����� ������ �� ��������.
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
