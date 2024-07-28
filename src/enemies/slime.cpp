#include "slime.hpp"
#include "expParticle.hpp"
#include "globals.h"
#include "player.h"
#include "render.h"
#include "utils.h"
#include <math.h>

static Texture2D slimeAtlas;

static bool loaded = false;

#define SLIME_SPEED 10
#define RENDER_PRIORITY 3
static i32 frameCount;

static void SlimeInit() {
    if (!loaded) {
        slimeAtlas = LoadTexture("assets/images/slimeSheet.png");
        frameCount = slimeAtlas.width / 16;
        loaded = true;
    }
}

static void render(void* slimeP) {
    Slime* slime = (Slime*)slimeP;
    v2 shadowPos = {slime->rect.x + slime->rect.width / 2,
                    slime->rect.y + slime->rect.height / 2};

    DrawEllipse(shadowPos.x, shadowPos.y + 7, slime->rect.width / 2.5,
                slime->rect.height / 4, (Color){0, 0, 0, 150});

    Rect src = {0, 0, 16, 16};
    src.x = (i32)slime->frame * 16;
    Rect dest = {slime->rect.x, slime->rect.y, 16, 16};

    DrawTexturePro(slimeAtlas, src, dest, (Vector2){0, 0}, 0, WHITE);
    slime->healthBar->render(GREEN, true);
}

void Slime::handleCollision() {
    if (CheckCollisionRecs(player->rect, rect)) {
        damagePlayer(1);
    }
}

void Slime::destroy() {
    ExpParticle::batchCreate((v2){rect.x, rect.y}, GREEN, 10, 5);
    removeRender((RenderData){(void*)this, render, RENDER_PRIORITY});
    delete healthBar;
    markedForDeletion = true;
}

void Slime::update() {
    v2 playerPos = {player->rect.x, player->rect.y};
    v2 slimePos = {rect.x, rect.y};
    f32 ang = getAngleToPoint(slimePos, playerPos);
    rect.x += cos(ang * DEG2RAD) * SLIME_SPEED * GetFrameTime();
    rect.y += sin(ang * DEG2RAD) * SLIME_SPEED * GetFrameTime();
    frame = fmodf(frame + 0.25, frameCount);

    healthBar->value = health;
    healthBar->updatePos(rect.x, rect.y);
    handleCollision();

    if (health <= 0) {
        destroy();
    }
}

Rect Slime::getCollider() { return rect; }

Slime::Slime(v2 pos) : GameObject("slime") {
    SlimeInit();
    rect = (Rect){pos.x, pos.y, 16, 16};
    frame = 0;
    maxHealth = 100;
    health = maxHealth;
    healthBar = new Bar(0, 0, maxHealth, false);
    addRender((RenderData){this, render, RENDER_PRIORITY});
    return;
}
