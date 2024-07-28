#include "button.hpp"
#include "expParticle.hpp"
#include "fuelMeter.hpp"
#include "globals.h"
#include "particleSystem.h"
#include "raylib.h"
#include "render.h"
#include "slime.hpp"
#include "slimeGhoul.hpp"
#include "task.h"
#include "upgradeCard.h"
#include "utils.h"
#include <math.h>
#include <stdio.h>

Font bestFont;
Image gameIcon;

#define UI_SCALE 1

typedef enum EnemyType {
    ENEMY_SLIME,
    ENEMY_SLIME_GHOUL,
} EnemyType;
i32 enemyTypes = 2;

Texture2D UITexture;
Texture2D botBar;
Bar* healthBar;
Bar* ExpBar;
f64 lastEnemySpawn = 0;
f32 enemiesPerSecond = 0.20;

Texture2D startButtonT;
Texture2D settingsButtonT;
Texture2D quitButtonT;
Texture2D mainBackground;

Button* startButton;
Button* settingsButton;
Button* quitButton;

i32 enemiesSpawned = 0;

v2 getRandEnemyPos(void) {
    f32 angle = GetRandomValue(0, 360);
    f32 dist = (screenWidth + screenHeight) / 4.0;
    v2 pos;
    pos.x = screenWidth / 2.0 + cos(angle) * dist;
    pos.y = screenHeight / 2.0 + sin(angle) * dist;
    printf("pos: %f, %f\n", pos.x, pos.y);
    return pos;
}

void mapEnemyTypeToSpawnFunc(EnemyType type) {
    v2 pos = getRandEnemyPos();
    switch (type) {
    case ENEMY_SLIME:
        new Slime(pos);
        break;
    case ENEMY_SLIME_GHOUL:
        new SlimeGhoul(pos);
        break;
    }
    return;
}

void enterPlanet();

void quitButtonFunc() { quit = true; }

void initMainMenu() {
    startButton = new Button(v2{200, 77}, startButtonT, enterPlanet);
    settingsButton = new Button(v2{187, 141}, settingsButtonT, NULL);
    quitButton = new Button(v2{200, 206}, quitButtonT, quitButtonFunc);

    state = MAIN_MENU;
}

void destroyMainMenu() {
    startButton->destroy();
    settingsButton->destroy();
    quitButton->destroy();
}

void spawnEnemies(void) {
    if (GetTime() - lastEnemySpawn >= 1.0 / enemiesPerSecond) {
        i32 enemy = GetRandomValue(0, enemyTypes - 1);
        mapEnemyTypeToSpawnFunc((EnemyType)enemy);
        lastEnemySpawn = GetTime();
        enemiesSpawned++;
    }
}

void drawUI(void) {
    v2 pos = {screenWidth / 2.0, 16 * UI_SCALE};
    Rect src = {0, 0, UITexture.width, UITexture.height};
    Rect dest = {pos.x, pos.y, src.width * UI_SCALE, src.height * UI_SCALE};
    v2 org = {src.width * UI_SCALE / 2, src.height * UI_SCALE / 2};
    DrawTexturePro(UITexture, src, dest, org, 0, WHITE);

    healthBar->value = player->health;
    ExpBar->value = player->exp;
    ExpBar->maxValue = player->expThreshold;

    healthBar->render(Color{255, 105, 97, 255}, false);
    ExpBar->render(Color{167, 199, 231, 255}, false);

    v2 thumbPos = pos; // thumbnail is 24x24
    thumbPos.x -= 12 * UI_SCALE;
    thumbPos.y -= 12 * UI_SCALE;

    drawPlanetThumbnail(thumbPos, currentPlanet);
    // DrawFPS(100, 100);

    DrawText(TextFormat("%d", player->level), 294, 6, 10, WHITE);
}

void enterPlanet() {
    // eventually planets will be preloaded and
    // selected from a menu, for now we just generate one

    currentPlanet = genPlanet(64, true);
    currentTerrain = genPlanetTerrain(currentPlanet);
    state = ON_PLANET;

    destroyMainMenu();
}

int main(void) {
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    SetTraceLogLevel(LOG_WARNING);
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
    gameIcon = LoadImage("assets/images/CosmicAscentIcon.png");
    SetWindowIcon(gameIcon);

    healthBar = new Bar(163, 16, 100, true);
    ExpBar = new Bar(275, 16, 100, true);

    // const u16 imgSize = 64;

    player = PlayerCreate(screenWidth / 2.0, screenHeight / 2.0);
    Sword* sword = createSword(player, &mouse, WHITE);
    player->weaponData = (WeaponData){sword, sword->use, sword->render};

    // Planet testPlanet = genPlanet(64, true);
    // testPlanet.pos = (v2){screenWidth - 350, screenHeight / 2.0 - 128};
    // PlanetTerrain* terrain = genPlanetTerrain(&testPlanet);

    ParticleSystem* testSys =
        createParticleSystem((v2){100, 200}, 10, 5, 4, 10);
    // state = ON_PLANET;

    // TEST:
    startButtonT = LoadTexture("assets/images/startButton.png");
    settingsButtonT = LoadTexture("assets/images/settingsButton.png");
    quitButtonT = LoadTexture("assets/images/quitButton.png");
    mainBackground = LoadTexture("assets/images/mainMenu.png");

    new FuelMeter(0, 0, 100);

    initMainMenu();

    while (!WindowShouldClose() && !quit) {

        f32 scale = MIN((f32)GetScreenWidth() / screenWidth,
                        (float)GetScreenHeight() / screenHeight);

        mouse = getScreenMousePos(&mouse, scale, screenWidth, screenHeight);
        player->update(player);
        ExpParticle::updateAll();
        GameObject::runAll();
        updateParticleSystem(testSys);
        runAllTasks();

        if (IsKeyPressed(KEY_F3)) {
            printRunningTasks();
        }

        switch (state) {
        case MAIN_MENU:
            break;
        case IN_SPACE:
            break;
        case ON_PLANET:
            spawnEnemies();
            break;
        case GAME_OVER:
            break;
        }

        BeginTextureMode(target);

        ClearBackground(BLACK);

        switch (state) {
        case MAIN_MENU:
            DrawTexture(mainBackground, 0, 0, WHITE);
            break;
        case IN_SPACE:
            break;
        case ON_PLANET:
            DrawTextureEx(currentTerrain->texture, (v2){}, 0, LEVEL_SCALE,
                          WHITE);

            player->render(player);
            ExpParticle::drawAll();
            drawParticleSystem(testSys);
            drawUI();
            break;
        case GAME_OVER:
            break;
        }

        renderAll();
        handleLevelupUI();

        // DrawText(TextFormat("Mouse: (%f, %f)", mouse.x, mouse.y), 0, 0,
        // 10,
        //          WHITE);

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
