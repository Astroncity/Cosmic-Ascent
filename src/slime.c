#include "slime.h"
#include "gameobject.h"
#include "globals.h"
#include "render.h"
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

static f32 getAngleToPlayer(Slime* slime) {
    v2 diff;
    diff.x = player->rect.x - slime->rect.x;
    diff.y = player->rect.y - slime->rect.y;
    return atan2(diff.y, diff.x) * RAD2DEG;
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
    slime->frame = fmodf(slime->frame + 0.25, frameCount);
    handleCollision(slime);
}

static Rect getCollider(GameObject* gameObject) {
    return ((Slime*)gameObject->obj)->rect;
}

Slime* SlimeCreate() {
    SlimeInit();
    Slime* slime = malloc(sizeof(Slime));
    slime->rect = (Rect){200, 100, 16, 16};
    slime->frame = 0;
    createGameObject("slime", slime, getCollider, update);
    addRender((RenderData){slime, render, RENDER_PRIORITY});
    return slime;
}
