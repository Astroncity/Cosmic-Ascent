#include "slime.h"
#include "gameobject.h"
#include "raylib.h"
#include "render.h"
#include <math.h>

static Texture2D slimeTexture;
static Player* player;

static bool loaded = false;

#define SLIME_SPEED 10
#define RENDER_PRIORITY 3
static v2* mouse;

void SlimeInit(Player* plr, v2* mousePos) {
    if (!loaded) {
        slimeTexture = LoadTexture("assets/images/slime.png");
        player = plr;
        mouse = mousePos;
        loaded = true;
    }
}

static f32 getAngleToPlayer(Slime* slime) {
    v2 diff;
    diff.x = player->rect.x - slime->rect.x;
    diff.y = player->rect.y - slime->rect.y;
    return atan2(diff.y, diff.x) * RAD2DEG;
}

static void render(void* slimeP) {
    Slime* slime = (Slime*)slimeP;
    DrawTexture(slime->texture, slime->rect.x, slime->rect.y, WHITE);
}

static void handleCollision(Slime* slime) {
    if (CheckCollisionRecs(player->rect, slime->rect)) {
        player->health -= 1;
    }
}

static void update(void* slimeP) {
    Slime* slime = (Slime*)slimeP;
    f32 ang = getAngleToPlayer(slime);
    slime->rect.x += cos(ang * DEG2RAD) * SLIME_SPEED * GetFrameTime();
    slime->rect.y += sin(ang * DEG2RAD) * SLIME_SPEED * GetFrameTime();
    handleCollision(slime);
}

static Rect getCollider(GameObject* gameObject) {
    return ((Slime*)gameObject->obj)->rect;
}

Slime* SlimeCreate() {
    Slime* slime = malloc(sizeof(Slime));
    slime->rect = (Rect){200, 100, 16, 16};
    slime->texture = slimeTexture;
    createGameObject("slime", slime, getCollider, update);
    addRender((RenderData){slime, render, RENDER_PRIORITY});
    return slime;
}
