#include "textEffects.hpp"
#include "gameobject.hpp"
#include <stdio.h>
#include <string.h>

void DrawText3D(Font font, const char* text, i32 x, i32 y, i32 size,
                Color color) {
    for (usize i = 0; i < 5; i++) {
        i16 tempA = (color.a);
        tempA -= i * 50;
        if (tempA < 0) tempA = 0;

        Color cl = (Color){color.r, color.g, color.b, (u8)tempA};
        DrawTextEx(font, text, (v2){(f32)x, y + (f32)i * 1.5}, size, 1, cl);
    }
}

class FlashingText : public GameObject {
  public:
    f32 start;
    f32 duration;
    v2 pos;
    const char* text;
    Color color;
    Font font;
    i32 size;
    bool decreasing;

    FlashingText(Font font, const char* text, v2 pos, i32 size, Color color,
                 f32 duration)
        : GameObject("flashingText", 100) {
        this->start = GetTime();
        this->duration = duration;
        this->pos = pos;
        this->text = strdup(text);
        this->color = color;
        this->font = font;
        this->size = size;
        this->decreasing = true;
    }

    void render() override;
    void update() override;
    void destroy() override;
    Rect getCollider() override;
};

char* strdup(const char* str) {
    size_t len = strlen(str) + 1; // +1 for null terminator
    char* copy = (char*)malloc(len);
    if (copy) {
        memcpy(copy, str, len);
    }
    return copy;
}

void FlashingText::render() { DrawTextEx(font, text, pos, size, 1, color); }
Rect FlashingText::getCollider() { return {}; }
void FlashingText::destroy() {
    removeRender(renderData);
    free((char*)text);
    markedForDeletion = true;
}

void FlashingText::update() {
    bool ended = GetTime() >= start + duration;

    if (ended && color.a == 0) {
        destroy();
        return;
    }

    i32 alpha = color.a;

    if (decreasing) {
        alpha -= 500 * GetFrameTime();
        if (alpha <= 0) {
            alpha = 0;
            decreasing = false;
        }
    } else {
        alpha += 500 * GetFrameTime();
        if (alpha >= 255) {
            alpha = 255;
            decreasing = true;
        }
    }

    color.a = alpha;
}

void DrawFlashingText(Font font, const char* text, v2 pos, i32 size,
                      Color color, f32 duration) {
    new FlashingText(font, text, pos, size, color, duration);
}
