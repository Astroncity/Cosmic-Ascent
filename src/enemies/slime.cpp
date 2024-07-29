#include "slime.hpp"
#include "expParticle.hpp"
#include "globals.hpp"
#include "raylib.h"
#include "render.hpp"
#include "utils.h"
#include <functional>
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

void Slime::render() {
    v2 shadowPos = {rect.x + rect.width / 2, rect.y + rect.height / 2};

    DrawEllipse(shadowPos.x, shadowPos.y + 7, rect.width / 2.5,
                rect.height / 4, (Color){0, 0, 0, 150});

    Rect src = {0, 0, 16, 16};
    src.x = (i32)frame * 16;
    Rect dest = {rect.x, rect.y, 16, 16};

    DrawTexturePro(slimeAtlas, src, dest, (Vector2){0, 0}, 0, WHITE);
    healthBar->render(GREEN, true);
}

void Slime::handleCollision() {
    if (CheckCollisionRecs(player->rect, rect)) {
        player->damage(1);
    }
}

void Slime::destroy() {
    ExpParticle::batchCreate((v2){rect.x, rect.y}, GREEN, 10, 5);
    delete healthBar;
    markedForDeletion = true;
    removeRender(renderData);
}

void Slime::update() {
    v2 playerPos = {player->rect.x, player->rect.y};
    v2 slimePos = {rect.x, rect.y};
    f32 ang = getAngleToPoint(slimePos, playerPos);
    rect.x += cos(ang * DEG2RAD) * SLIME_SPEED * GetFrameTime();
    rect.y += sin(ang * DEG2RAD) * SLIME_SPEED * GetFrameTime();
    frame = fmodf(frame + 25 * GetFrameTime(), frameCount);

    healthBar->value = health;
    healthBar->updatePos(rect.x, rect.y);
    handleCollision();

    if (health <= 0) {
        destroy();
    }
}

Rect Slime::getCollider() { return rect; }

Slime::Slime(v2 pos) : GameObject("slime", RENDER_PRIORITY) {
    SlimeInit();
    rect = (Rect){pos.x, pos.y, 16, 16};
    frame = 0;
    maxHealth = 100;
    health = maxHealth;
    healthBar = new Bar(0, 0, maxHealth, false);
    return;
}
