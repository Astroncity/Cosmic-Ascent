#include "slime.h"
#include "bar.h"
#include "gameobject.h"
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
    BarRender(slime->healthBar, GREEN, true);
}

static void handleCollision(Slime* slime) {
    if (CheckCollisionRecs(player->rect, slime->rect)) {
        damagePlayer(1);
    }
}

static void updateBar(Slime* slime) {
    Bar* bar = slime->healthBar;
    bar->rect.x = slime->rect.x;
    bar->rect.y = slime->rect.y;
    bar->value = slime->health;
}

static void destroy(Slime* self) {
    removeRender((RenderData){self, render, RENDER_PRIORITY});
    removeGameObject(self->gameobject);
    free(self->healthBar);
    free(self);
}

static void update(void* slimeP) {
    Slime* slime = (Slime*)slimeP;
    v2 playerPos = {player->rect.x, player->rect.y};
    v2 slimePos = {slime->rect.x, slime->rect.y};
    f32 ang = getAngleToPoint(slimePos, playerPos);
    slime->rect.x += cos(ang * DEG2RAD) * SLIME_SPEED * GetFrameTime();
    slime->rect.y += sin(ang * DEG2RAD) * SLIME_SPEED * GetFrameTime();
    slime->frame = fmodf(slime->frame + 0.25, frameCount);

    updateBar(slime);
    handleCollision(slime);

    if (slime->health <= 0) {
        destroy(slime);
    }
}

static Rect getCollider(GameObject* gameObject) {
    return ((Slime*)gameObject->obj)->rect;
}

Slime* SlimeCreate() {
    SlimeInit();
    Slime* slime = malloc(sizeof(Slime));
    slime->rect = (Rect){200, 100, 16, 16};
    slime->frame = 0;
    slime->maxHealth = 100;
    slime->health = slime->maxHealth;
    slime->healthBar = BarCreate(0, 0, slime->maxHealth, false);
    slime->gameobject =
        createGameObject("slime", slime, getCollider, update);
    addRender((RenderData){slime, render, RENDER_PRIORITY});
    return slime;
}
