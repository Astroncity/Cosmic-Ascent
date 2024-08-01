#include "slimeGhoul.hpp"
#include "expParticle.hpp"
#include "globals.hpp"
#include "raylib.h"
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

void SlimeGhoul::render() {
    v2 shadowPos = {rect.x + rect.width / 2, rect.y + rect.height / 2};

    DrawEllipse(shadowPos.x, shadowPos.y + 7, rect.width / 2.5,
                rect.height / 4, (Color){0, 0, 0, 150});

    Rect src = {0, 0, 16, 16};
    src.x = (i32)frame * 16;
    Rect dest = {rect.x, rect.y, 16, 16};

    DrawTexturePro(slimeGhoulAtlas, src, dest, (Vector2){0, 0}, 0, WHITE);
    healthBar->render(GREEN, true);
}

void SlimeGhoul::handleCollision() {
    if (CheckCollisionRecs(player->getCollider(), rect)) {
        player->damage(1);
    }
}

void SlimeGhoul::destroy() {
    GameObject::destroy();
    ExpParticle::batchCreate((v2){rect.x, rect.y}, GREEN, 10, 5);
    delete healthBar;
}

void SlimeGhoul::update() {
    v2 playerPos = {player->rect.x, player->rect.y};
    v2 slimePos = {rect.x, rect.y};
    f32 ang = getAngleToPoint(slimePos, playerPos);

    rect.x += cos(ang * DEG2RAD) * GHOUL_SPEED * GetFrameTime();
    rect.y += sin(ang * DEG2RAD) * GHOUL_SPEED * GetFrameTime();

    frame = fmodf(frame + 10 * GetFrameTime(), frameCount);
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
        new SlimeProjectile(pos, ang, 50);
    }
}

Rect SlimeGhoul::getCollider() { return rect; }

SlimeGhoul::SlimeGhoul(v2 pos) : GameObject("slimeGhoul", RENDER_PRIORITY) {
    init();
    rect = (Rect){pos.x, pos.y, 16, 16};
    frame = 0;
    projectileTimer = 0;
    maxHealth = 50;
    health = maxHealth;
    healthBar = new Bar(0, 0, maxHealth, false);
}
