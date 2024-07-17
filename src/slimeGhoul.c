#include "slimeGhoul.h"
#include "gameobject.h"
#include "globals.h"
#include "player.h"
#include "render.h"
#include "utils.h"
#include <math.h>

static Texture2D slimeGhoulAtlas;
static Texture2D projectile;
static bool loaded = false;

#define GHOUL_SPEED 10
#define RENDER_PRIORITY 3
static i32 frameCount;

static void init() {
    if (!loaded) {
        slimeGhoulAtlas = LoadTexture("assets/images/slimeGhoul.png");
        projectile = LoadTexture("assets/images/slimeProjectile.png");
        frameCount = slimeGhoulAtlas.width / 16;
        loaded = true;
    }
}

static void render(void* slimeP) {
    SlimeGhoul* ghoul = (SlimeGhoul*)slimeP;
    v2 shadowPos = {ghoul->rect.x + ghoul->rect.width / 2,
                    ghoul->rect.y + ghoul->rect.height / 2};

    DrawEllipse(shadowPos.x, shadowPos.y + 7, ghoul->rect.width / 2.5,
                ghoul->rect.height / 4, (Color){0, 0, 0, 150});

    Rect src = {0, 0, 16, 16};
    src.x = (i32)ghoul->frame * 16;
    Rect dest = {ghoul->rect.x, ghoul->rect.y, 16, 16};

    DrawTexturePro(slimeGhoulAtlas, src, dest, (Vector2){0, 0}, 0, WHITE);
}

static void handleCollision(SlimeGhoul* slime) {
    if (CheckCollisionRecs(player->rect, slime->rect)) {
        damagePlayer(1);
    }
}

static void update(void* slimeP) {
    SlimeGhoul* ghoul = (SlimeGhoul*)slimeP;
    v2 playerPos = {player->rect.x, player->rect.y};
    v2 slimePos = {ghoul->rect.x, ghoul->rect.y};
    f32 ang = getAngleToPoint(slimePos, playerPos);

    ghoul->rect.x += cos(ang * DEG2RAD) * GHOUL_SPEED * GetFrameTime();
    ghoul->rect.y += sin(ang * DEG2RAD) * GHOUL_SPEED * GetFrameTime();

    ghoul->frame = fmodf(ghoul->frame + 0.10, frameCount);
    handleCollision(ghoul);
}

static Rect getCollider(GameObject* gameObject) {
    return ((SlimeGhoul*)gameObject->obj)->rect;
}

SlimeGhoul* SlimeGhoulCreate() {
    init();

    SlimeGhoul* ghoul = malloc(sizeof(SlimeGhoul));
    ghoul->rect = (Rect){100, 100, 16, 16};
    ghoul->frame = 0;

    createGameObject("slime ghoul", ghoul, getCollider, update);
    addRender((RenderData){ghoul, render, RENDER_PRIORITY});
    return ghoul;
}
