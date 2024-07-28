#include "slimeProjectile.h"
#include "gameobject.h"
#include "globals.h"
#include "player.h"
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

static void destroy(SlimeProjectile* proj) {
    removeRender(proj->renderData);
    free(proj);
}

static void update(void* projP) {
    SlimeProjectile* proj = (SlimeProjectile*)projP;

    proj->pos.x +=
        cos(proj->direction * DEG2RAD) * proj->speed * GetFrameTime();
    proj->pos.y +=
        sin(proj->direction * DEG2RAD) * proj->speed * GetFrameTime();
    if (CheckCollisionCircleRec(proj->pos, 8, getPlayerCollider(player))) {
        damagePlayer(1);
        destroy(proj);
        return;
    }

    // bounds check
    if (proj->pos.x < 0 || proj->pos.x > screenWidth || proj->pos.y < 0 ||
        proj->pos.y > screenHeight) {
        destroy(proj);
    }
}

static void render(void* projP) {
    update(projP);
    SlimeProjectile* proj = (SlimeProjectile*)projP;
    DrawTexturePro(slimeProjectile,
                   (Rect){0, 0, (f32)slimeProjectile.width,
                          (f32)slimeProjectile.height},
                   (Rect){proj->pos.x, proj->pos.y, 8, 8}, (Vector2){},
                   proj->direction, WHITE);
}

SlimeProjectile* SlimeProjectileCreate(v2 start, f32 dir, f32 speed) {
    init();
    SlimeProjectile* proj =
        (SlimeProjectile*)malloc(sizeof(SlimeProjectile));
    proj->pos = start;
    proj->direction = dir;
    proj->speed = speed;
    proj->renderData = (RenderData){proj, render, RENDER_PRIORITY};
    addRender(proj->renderData);
    return proj;
}
