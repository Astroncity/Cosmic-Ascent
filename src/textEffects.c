#include "textEffects.h"
#include "task.h"
#include <stdio.h>
#include <string.h>

void DrawText3D(Font font, const char* text, i32 x, i32 y, i32 size,
                Color color) {
    for (usize i = 0; i < 5; i++) {
        i16 tempA = (i16)color.a;
        tempA -= i * 50;
        if (tempA < 0) tempA = 0;

        Color cl = (Color){color.r, color.g, color.b, (u8)tempA};
        DrawTextEx(font, text, (v2){(f32)x, y + (f32)i * 1.5}, size, 1, cl);
    }
}

char* strdup(const char* str) {
    size_t len = strlen(str) + 1; // +1 for null terminator
    char* copy = malloc(len);
    if (copy) {
        memcpy(copy, str, len);
    }
    return copy;
}

typedef struct FlashingTextData {
    f32 start;
    f32 duration;
    v2 pos;
    const char* text;
    Color color;
    Font font;
    i32 size;
    bool decreasing;
} FlashingTextData;

static void flashingTextTask(TASK_PARAMS) {
    FlashingTextData* data = (FlashingTextData*)taskData;

    bool ended = GetTime() >= data->start + data->duration;

    if (ended && data->color.a == 0) {
        task->setForDeletion = true;
        free((char*)data->text);
        free(data);
        return;
    }

    DrawTextEx(data->font, data->text, data->pos, data->size, 1,
               data->color);

    i32 alpha = data->color.a;

    if (data->decreasing) {
        alpha -= 500 * GetFrameTime();
        if (alpha <= 0) {
            alpha = 0;
            data->decreasing = false;
        }
    } else {
        alpha += 500 * GetFrameTime();
        if (alpha >= 255) {
            alpha = 255;
            data->decreasing = true;
        }
    }

    data->color.a = alpha;
}

void DrawFlashingText(Font font, const char* text, v2 pos, i32 size,
                      Color color, f32 duration) {

    FlashingTextData* data = malloc(sizeof(FlashingTextData));
    data->start = GetTime();
    data->duration = duration;
    data->pos = pos;
    data->text = strdup(text);
    data->color = color;
    data->font = font;
    data->size = size;
    data->decreasing = true;

    createTask(data, flashingTextTask);
}
