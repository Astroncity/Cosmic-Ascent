#include "slimeProjectile.hpp"
#include "globals.hpp"
#include "raylib.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

static Texture2D slimeProjectile;
static bool loaded = false;

#define RENDER_PRIORITY 2

static void init() {
    if (!loaded) {
        slimeProjectile = LoadTexture("assets/images/slimeProjectile.png");
        loaded = true;
    }
}

void SlimeProjectile::destroy() { GameObject::destroy(); }

Rect SlimeProjectile::getCollider() { return {}; }

void SlimeProjectile::update() {

    pos.x += cos(direction * DEG2RAD) * speed * GetFrameTime();
    pos.y += sin(direction * DEG2RAD) * speed * GetFrameTime();
    if (CheckCollisionCircleRec(pos, 8, player->getCollider())) {
        player->damage(1);
        destroy();
        return;
    }

    // bounds check
    if (pos.x < 0 || pos.x > screenWidth || pos.y < 0 ||
        pos.y > screenHeight) {
        destroy();
    }
}

void SlimeProjectile::render() {
    DrawTexturePro(slimeProjectile,
                   {0, 0, slimeProjectile.width, slimeProjectile.height},
                   {pos.x, pos.y, 8, 8}, {}, direction, WHITE);
}

SlimeProjectile::SlimeProjectile(v2 start, f32 dir, f32 speed)
    : GameObject("slimeProjectile", RENDER_PRIORITY) {
    init();
    pos = start;
    direction = dir;
    this->speed = speed;
}
