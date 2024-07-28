#include "slimeGhoul.hpp"
#include "expParticle.h"
#include "gameobject.h"
#include "globals.h"
#include "player.h"
#include "raylib.h"
#include "render.h"
#include "slimeProjectile.h"
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

static void handleCollision(SlimeGhoul* slime) {
    if (CheckCollisionRecs(getPlayerCollider(player), slime->rect)) {
        damagePlayer(1);
    }
}

static void updateBar(SlimeGhoul* slime) {
    Bar* bar = slime->healthBar;
    bar->value = slime->health;
    bar->updatePos(slime->rect.x, slime->rect.y);
}

static void destroy(SlimeGhoul* self) {
    ExpParticleBatchCreate((v2){self->rect.x, self->rect.y}, GREEN, 10, 5);
    removeRender((RenderData){self, render, RENDER_PRIORITY});
    removeGameObject(self->gameobject);
    free(self->healthBar);
    free(self);
}

static void update(void* ghoulP) {
    SlimeGhoul* ghoul = (SlimeGhoul*)ghoulP;
    v2 playerPos = {player->rect.x, player->rect.y};
    v2 slimePos = {ghoul->rect.x, ghoul->rect.y};
    f32 ang = getAngleToPoint(slimePos, playerPos);

    ghoul->rect.x += cos(ang * DEG2RAD) * GHOUL_SPEED * GetFrameTime();
    ghoul->rect.y += sin(ang * DEG2RAD) * GHOUL_SPEED * GetFrameTime();

    ghoul->frame = fmodf(ghoul->frame + 0.10, frameCount);
    updateBar(ghoul);
    handleCollision(ghoul);
    ghoul->projectileTimer += GetFrameTime();

    if (ghoul->health <= 0) {
        destroy(ghoul);
    }

    if (ghoul->projectileTimer > GHOUL_PROJ_COOLDOWN) {
        ghoul->projectileTimer = 0;
        v2 pos = {ghoul->rect.x, ghoul->rect.y};
        SlimeProjectileCreate(pos, ang, 50);
    }
}

static Rect getCollider(GameObject* gameObject) {
    return ((SlimeGhoul*)gameObject->obj)->rect;
}

SlimeGhoul* SlimeGhoulCreate(v2 pos) {
    init();

    SlimeGhoul* ghoul = (SlimeGhoul*)malloc(sizeof(SlimeGhoul));
    ghoul->rect = (Rect){pos.x, pos.y, 16, 16};
    ghoul->frame = 0;
    ghoul->projectileTimer = 0;
    ghoul->maxHealth = 50;
    ghoul->health = ghoul->maxHealth;
    ghoul->healthBar = new Bar(0, 0, ghoul->maxHealth, false);

    ghoul->gameobject =
        createGameObject("slime ghoul", ghoul, getCollider, update);
    addRender((RenderData){ghoul, render, RENDER_PRIORITY});
    return ghoul;
}
