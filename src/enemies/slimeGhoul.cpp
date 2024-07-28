#include "slimeGhoul.hpp"
#include "expParticle.hpp"
#include "globals.h"
#include "player.h"
#include "raylib.h"
#include "render.h"
#include "slimeProjectile.hpp"
#include "utils.h"
#include <math.h>

static Texture2D slimeGhoulAtlas;
static bool loaded = false;

#define GHOUL_SPEED 10
#define RENDER_PRIORITY 3
#define GHOUL_PROJ_COOLDOWN 1

static i32 frameCount;

static void init() {
    if (!loaded) {
        slimeGhoulAtlas = LoadTexture("assets/images/slimeGhoul.png");
        frameCount = slimeGhoulAtlas.width / 16;
        loaded = true;
    }
}

static void render(void* ghoulP) {
    SlimeGhoul* ghoul = (SlimeGhoul*)ghoulP;
    v2 shadowPos = {ghoul->rect.x + ghoul->rect.width / 2,
                    ghoul->rect.y + ghoul->rect.height / 2};

    DrawEllipse(shadowPos.x, shadowPos.y + 7, ghoul->rect.width / 2.5,
                ghoul->rect.height / 4, (Color){0, 0, 0, 150});

    Rect src = {0, 0, 16, 16};
    src.x = (i32)ghoul->frame * 16;
    Rect dest = {ghoul->rect.x, ghoul->rect.y, 16, 16};

    DrawTexturePro(slimeGhoulAtlas, src, dest, (Vector2){0, 0}, 0, WHITE);
    ghoul->healthBar->render(GREEN, true);
}

void SlimeGhoul::handleCollision() {
    if (CheckCollisionRecs(getPlayerCollider(player), rect)) {
        damagePlayer(1);
    }
}

void SlimeGhoul::destroy() {
    ExpParticle::batchCreate((v2){rect.x, rect.y}, GREEN, 10, 5);
    removeRender((RenderData){(void*)this, render, RENDER_PRIORITY});
    delete healthBar;
    markedForDeletion = true;
}

void SlimeGhoul::update() {
    v2 playerPos = {player->rect.x, player->rect.y};
    v2 slimePos = {rect.x, rect.y};
    f32 ang = getAngleToPoint(slimePos, playerPos);

    rect.x += cos(ang * DEG2RAD) * GHOUL_SPEED * GetFrameTime();
    rect.y += sin(ang * DEG2RAD) * GHOUL_SPEED * GetFrameTime();

    frame = fmodf(frame + 0.10, frameCount);
    healthBar->value = health;
    healthBar->updatePos(rect.x, rect.y);
    handleCollision();
    projectileTimer += GetFrameTime();

    if (health <= 0) {
        destroy();
    }

    if (projectileTimer > GHOUL_PROJ_COOLDOWN) {
        projectileTimer = 0;
        v2 pos = {rect.x, rect.y};
        SlimeProjectileCreate(pos, ang, 50);
    }
}

Rect SlimeGhoul::getCollider() { return rect; }

SlimeGhoul::SlimeGhoul(v2 pos) : GameObject("slimeGhoul") {
    init();
    rect = (Rect){pos.x, pos.y, 16, 16};
    frame = 0;
    projectileTimer = 0;
    maxHealth = 50;
    health = maxHealth;
    healthBar = new Bar(0, 0, maxHealth, false);

    addRender((RenderData){this, render, RENDER_PRIORITY});
}
