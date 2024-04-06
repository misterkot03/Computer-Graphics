// graphics.c
#include "../headers/graphics.h"

int currentFrame = 0;          // Текущий кадр анимации
// Функция отрисовки одного тайла
void RenderTile(Tile *tile, float x, float y, float width, float height) {
    if (tile->type == TILE_TYPE_NONE) return;

    glBindTexture(GL_TEXTURE_2D, tile->texture);
    glEnable(GL_TEXTURE_2D);
    glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 0.0f); glVertex2f(x, y);
        glTexCoord2f(1.0f, 0.0f); glVertex2f(x + width, y);
        glTexCoord2f(1.0f, 1.0f); glVertex2f(x + width, y + height);
        glTexCoord2f(0.0f, 1.0f); glVertex2f(x, y + height);
    glEnd();
    glDisable(GL_TEXTURE_2D);
}




// Функция рендеринга анимации
void RenderSpriteAnimation(GLuint texture, float posX, float posY, float width, float height, float scale) {
    float frameWidth = 1.0f / 8;
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
