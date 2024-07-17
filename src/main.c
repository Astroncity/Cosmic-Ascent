#include "bar.h"
#include "defs.h"
#include "expParticle.h"
#include "gameobject.h"
#include "globals.h"
#include "render.h"
#include "slime.h"
#include "slimeGhoul.h"
#include "task.h"
#include "upgradeCard.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>

Font bestFont;

#define UI_SCALE 1

Texture2D UITexture;
Texture2D botBar;
Bar* healthBar;
Bar* ExpBar;

void drawUI() {
    v2 pos = {screenWidth / 2.0, 16 * UI_SCALE};
    Rect src = {0, 0, UITexture.width, UITexture.height};
    Rect dest = {pos.x, pos.y, src.width * UI_SCALE, src.height * UI_SCALE};
    v2 org = {src.width * UI_SCALE / 2, src.height * UI_SCALE / 2};
    DrawTexturePro(UITexture, src, dest, org, 0, WHITE);

    healthBar->value = player->health;
    ExpBar->value = player->exp;
    ExpBar->maxValue = player->expThreshold;

    BarRender(healthBar, (Color){255, 105, 97, 255}, false);
    BarRender(ExpBar, (Color){167, 199, 231, 255}, false);

    v2 thumbPos = pos; // thumbnail is 24x24
    thumbPos.x -= 12 * UI_SCALE;
    thumbPos.y -= 12 * UI_SCALE;

    drawPlanetThumbnail(thumbPos, currentPlanet);
    // DrawFPS(100, 100);

    DrawText(TextFormat("%d", player->level), 294, 6, 10, WHITE);
}

int main(void) {
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    SetTraceLogLevel(LOG_ERROR);
    InitWindow(screenWidth, screenHeight, "Planet Generation Test");
    InitAudioDevice();
    SetMasterVolume(1);
    SetTargetFPS(144);
    SetWindowSize(1920, 1080);

    RenderTexture2D target = LoadRenderTexture(screenWidth, screenHeight);
    SetTextureFilter(target.texture, TEXTURE_FILTER_POINT);

    bestFont = LoadFontEx("assets/fonts/spaceMono.ttf", 100, NULL, 0);
    UITexture = LoadTexture("assets/images/UI.png");
    botBar = LoadTexture("assets/images/barBot.png");

    healthBar = BarCreate(163, 16, 100, true);
    ExpBar = BarCreate(275, 16, 100, true);

    // const u16 imgSize = 64;

    player = PlayerCreate(screenWidth / 2.0, screenHeight / 2.0);
    Sword* sword = createSword(player, &mouse, WHITE);
    player->weaponData = (WeaponData){sword, sword->use};

    Planet testPlanet = genPlanet(64, true);
    currentPlanet = &testPlanet;
    testPlanet.pos = (v2){screenWidth - 350, screenHeight / 2.0 - 128};
    PlanetTerrain* terrain = genPlanetTerrain(&testPlanet);
    currentTerrain = terrain;

    SlimeCreate();
    SlimeGhoulCreate();

    while (!WindowShouldClose()) {

        f32 scale = MIN((f32)GetScreenWidth() / screenWidth,
                        (float)GetScreenHeight() / screenHeight);

        mouse = getScreenMousePos(&mouse, scale, screenWidth, screenHeight);
        player->update(player);
        ExpParticleUpdateAll();
        runGameObjects();
        // updateParticleSystem(testSys);
        runAllTasks();

        BeginTextureMode(target);

        ClearBackground(BLACK);
        DrawTextureEx(terrain->texture, (v2){}, 0, LEVEL_SCALE, WHITE);

        player->render(player);
        renderAll();
        ExpParticleDrawAll();
        // drawParticleSystem(testSys);
        drawUI();
        handleLevelupUI();

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
