#include "defs.h"
#include "planet.h"
#include "raylib.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

Vector2 mouse;
Font bestFont;

Vector2 v2Clamp(Vector2 vec, Vector2 min, Vector2 max) {
    return (Vector2){MIN(MAX(vec.x, min.x), max.x),
                     MIN(MAX(vec.y, min.y), max.y)};
}

void DrawText3D(const char *text, i32 x, i32 y, i32 size, Color color) {
    for (usize i = 0; i < 5; i++) {
        i16 tempA = (i16)color.a;
        tempA -= i * 50;
        if (tempA < 0)
            tempA = 0;

        Color cl = (Color){color.r, color.g, color.b, (u8)tempA};
        DrawTextEx(bestFont, text, (Vector2){(f32)x, y + (f32)i * 2}, size, 1,
                   cl);
    }
}

void drawScaledWindow(RenderTexture2D target, f32 sw, f32 sh, f32 scale) {
    f32 tw = (f32)target.texture.width;
    f32 th = (f32)target.texture.height;
    Rectangle rect1 = {0.0f, 0.0f, tw, -th};
    f32 x = (GetScreenWidth() - (sw * scale)) * 0.5f;
    f32 y = (GetScreenHeight() - (sh * scale)) * 0.5f;

    Rectangle rect2 = {x, y, sw * scale, sh * scale};

    DrawTexturePro(target.texture, rect1, rect2, (Vector2){0, 0}, 0.0f, WHITE);
}

Vector2 getScreenMousePos(f32 scale, i32 sw, i32 sh) {
    Vector2 mouseOLD = GetMousePosition();
    mouse.x = (mouseOLD.x - (GetScreenWidth() - (sw * scale)) * 0.5f) / scale;
    mouse.y = (mouseOLD.y - (GetScreenHeight() - (sh * scale)) * 0.5f) / scale;
    mouse = v2Clamp(mouse, (Vector2){0, 0}, (Vector2){(f32)sw, (f32)sh});

    return mouse;
}

int main(void) {
    const i32 screenWidth = 1920;
    const i32 screenHeight = 1080;

    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(screenWidth, screenHeight, "Planet Generation Test");
    InitAudioDevice();
    SetMasterVolume(0);
    SetTargetFPS(144);

    RenderTexture2D target = LoadRenderTexture(screenWidth, screenHeight);
    SetTextureFilter(target.texture, TEXTURE_FILTER_BILINEAR);

    bestFont = LoadFontEx("fonts/spaceMono.ttf", 100, NULL, 0);

    // TEST: perlin noise test

    const u16 imgSize = 64;

    f64 start, end;
    start = clock();

    Planet testPlanets[10];

    for (usize i = 0; i < 10; i++) {
        testPlanets[i] = genPlanet(imgSize, true);
    }

    end = clock();
    double elapsed = (double)(end - start) / CLOCKS_PER_SEC;
    printf("Time taken to generate 10 planets: %fs\n", elapsed);

    while (!WindowShouldClose()) {
        f32 scale = MIN((f32)GetScreenWidth() / screenWidth,
                        (float)GetScreenHeight() / screenHeight);

        mouse = getScreenMousePos(scale, screenWidth, screenHeight);

        BeginTextureMode(target);

        ClearBackground(BLACK);
        DrawFPS(100, 100);

        for (usize i = 0; i < 10; i++) {
            testPlanets[i].pos =
                (Vector2){(f32)(i % 5) * 200, (f32)(i / 5.0) * 200};
            drawPlanet(&testPlanets[i]);
        }

        EndTextureMode();

        BeginDrawing();
        ClearBackground(BLACK);
        drawScaledWindow(target, screenWidth, screenHeight, scale);
        EndDrawing();
    }
}
