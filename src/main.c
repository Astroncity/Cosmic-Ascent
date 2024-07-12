#include "bar.h"
#include "defs.h"
#include "gameobject.h"
#include "planet.h"
#include "planetTerrain.h"
#include "player.h"
#include "raylib.h"
#include "render.h"
#include "task.h"
#include <stdio.h>
#include <stdlib.h>

v2 mouse;
Font bestFont;

#define UI_SCALE 1

Planet* currentPlanet;
PlanetTerrain* currentTerrain;

Texture2D UITexture;
Texture2D botBar;
Bar* healthBar;
Player* player;

v2 v2Clamp(v2 vec, v2 min, v2 max) {
    return (v2){MIN(MAX(vec.x, min.x), max.x),
                MIN(MAX(vec.y, min.y), max.y)};
}

void DrawText3D(const char* text, i32 x, i32 y, i32 size, Color color) {
    for (usize i = 0; i < 5; i++) {
        i16 tempA = (i16)color.a;
        tempA -= i * 50;
        if (tempA < 0) tempA = 0;

        Color cl = (Color){color.r, color.g, color.b, (u8)tempA};
        DrawTextEx(bestFont, text, (v2){(f32)x, y + (f32)i * 2}, size, 1,
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

    DrawTexturePro(target.texture, rect1, rect2, (v2){0, 0}, 0.0f, WHITE);
}

v2 getScreenMousePos(f32 scale, i32 sw, i32 sh) {
    v2 mouseOLD = GetMousePosition();
    mouse.x =
        (mouseOLD.x - (GetScreenWidth() - (sw * scale)) * 0.5f) / scale;
    mouse.y =
        (mouseOLD.y - (GetScreenHeight() - (sh * scale)) * 0.5f) / scale;
    mouse = v2Clamp(mouse, (v2){0, 0}, (v2){(f32)sw, (f32)sh});

    return mouse;
}

void drawUI() {
    v2 pos = {screenWidth / 2.0, 16 * UI_SCALE};
    Rectangle src = {0, 0, UITexture.width, UITexture.height};
    Rectangle dest = {pos.x, pos.y, src.width * UI_SCALE,
                      src.height * UI_SCALE};
    v2 org = {src.width * UI_SCALE / 2, src.height * UI_SCALE / 2};
    DrawTexturePro(UITexture, src, dest, org, 0, WHITE);

    healthBar->value = player->health;
    BarRender(healthBar, (Color){255, 105, 97, 255});
    v2 thumbPos = pos; // thumbnail is 24x24
    thumbPos.x -= 12 * UI_SCALE;
    thumbPos.y -= 12 * UI_SCALE;

    drawPlanetThumbnail(thumbPos, currentPlanet);
    // DrawFPS(100, 100);
}

int main(void) {
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    SetTraceLogLevel(LOG_ERROR);
    InitWindow(screenWidth, screenHeight, "Planet Generation Test");
    InitAudioDevice();
    SetMasterVolume(0);
    SetTargetFPS(144);
    SetWindowSize(1920, 1080);

    RenderTexture2D target = LoadRenderTexture(screenWidth, screenHeight);
    SetTextureFilter(target.texture, TEXTURE_FILTER_POINT);

    bestFont = LoadFontEx("assets/fonts/spaceMono.ttf", 100, NULL, 0);
    UITexture = LoadTexture("assets/images/UI.png");
    botBar = LoadTexture("assets/images/barBot.png");

    healthBar = BarCreate(163, 16, 100, true);

    // const u16 imgSize = 64;

    player = PlayerCreate(screenWidth / 2.0, screenHeight / 2.0);
    Sword* sword = createSword(player, &mouse, WHITE);
    player->weaponData = (WeaponData){sword, sword->use};

    Planet testPlanet = genPlanet(64, true);
    currentPlanet = &testPlanet;
    testPlanet.pos = (v2){screenWidth - 350, screenHeight / 2.0 - 128};
    PlanetTerrain* terrain = genPlanetTerrain(&testPlanet);
    currentTerrain = terrain;

    while (!WindowShouldClose()) {

        f32 scale = MIN((f32)GetScreenWidth() / screenWidth,
                        (float)GetScreenHeight() / screenHeight);

        mouse = getScreenMousePos(scale, screenWidth, screenHeight);
        player->update(player);
        runAllTasks();
        runGameObjects();
        // updateParticleSystem(testSys);

        BeginTextureMode(target);

        ClearBackground(BLACK);
        DrawTextureEx(terrain->texture, (v2){}, 0, LEVEL_SCALE, WHITE);

        player->render(player);
        renderAll();
        // drawParticleSystem(testSys);
        drawUI();

        DrawText(TextFormat("Mouse: (%f, %f)", mouse.x, mouse.y), 0, 0, 10,
                 WHITE);

        // draw horizontal and vertical line for allignment
        // DrawLine(0, screenHeight / 2, screenWidth, screenHeight / 2,
        // RED); DrawLine(screenWidth / 2, 0, screenWidth / 2, screenHeight,
        // RED); drawPlanet(&testPlanet, 1);
        EndTextureMode();

        BeginDrawing();
        ClearBackground(BLACK);
        drawScaledWindow(target, screenWidth, screenHeight, scale);
        EndDrawing();
    }
}
