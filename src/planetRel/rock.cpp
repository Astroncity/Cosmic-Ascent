#include "rock.hpp"
#include "expParticle.h"
#include "gameobject.h"
#include "raylib.h"

const char* TEXTURE_PATH = "assets/images/rock.png";
const i32 RENDER_PRIORITY = 5;
const i32 ROCK_EXP = 2;
Texture2D rockTexture;
Sound rockBreakfx;
Sound rockHitfx;

Color shadowTint = {0, 0, 0, 150};

static Rect getRockCollider(GameObject* self) {
    Rock* rock = (Rock*)self->obj;
    Rect cpy = rock->rect;
    cpy.x -= rock->rect.width / 2;
    cpy.y -= rock->rect.height / 2;
    return cpy;
}

static void drawCol(Rock* rock) {
    Rect cpy = rock->rect;
    cpy.x -= rock->rect.width / 2;
    cpy.y -= rock->rect.height / 2;
    DrawRectangleLines(cpy.x, cpy.y, cpy.width, cpy.height, GREEN);
}

static void update(void* rockP) {
    Rock* rock = (Rock*)rockP;
    if (rock->invulnerableTimer > 0) {
        rock->invulnerableTimer -= GetFrameTime();
    }
}

static void render(void* rockP) {
    Rock* rock = (Rock*)rockP;

    Rect dest = rock->rect;

    Rect src = {0, 0, rock->rect.width, rock->rect.height};
    v2 org = {dest.width / 2, dest.height / 2};
    // shadow
    // DrawEllipse(rock->rect.x, rock->rect.y + 7, dest.width / 2,
    // dest.height / 4, shadowTint);
    Rect shadowDest = dest;
    shadowDest.y += 3;
    DrawTexturePro(rock->texture, src, shadowDest, org, 0, shadowTint);

    DrawTexturePro(rock->texture, src, dest, org, 0, rock->cl);
    rock->healthBar->render(rock->cl, true);
    if (false) drawCol(rock);
}

static void destroy(Rock* self) {
    if (!IsSoundReady(rockBreakfx)) {
        rockBreakfx = LoadSound("assets/sounds/rockBreakfx.wav");
        SetSoundVolume(rockBreakfx, 0.45);
    }
    PlaySound(rockBreakfx);

    ExpParticleBatchCreate((Vector2){self->rect.x, self->rect.y}, self->cl,
                           ROCK_EXP, 10);

    removeRender(self->renderData);
    removeGameObject(self->gameobject);
    free(self);
}

static bool hit(Rock* self) {
    if (self->invulnerableTimer > 0) {
        return false;
    }

    if (!IsSoundReady(rockHitfx)) {
        rockHitfx = LoadSound("assets/sounds/rockHitfx.wav");
        SetSoundVolume(rockHitfx, 0.45);
    }
    PlaySound(rockHitfx);

    self->invulnerableTimer = 0.1;
    self->health -= 1;
    self->healthBar->value = self->health;
    if (self->health <= 0) {
        destroy(self);
        return true;
    }
    return false;
}

Rock* createRock(f32 x, f32 y, Color cl) {
    if (rockTexture.id == 0) {
        Image temp = LoadImage(TEXTURE_PATH);
        rockTexture = LoadTextureFromImage(temp);
    }

    Rock* rock = (Rock*)malloc(sizeof(Rock));
    rock->texture = rockTexture;
    rock->rect = (Rect){x, y, rockTexture.width, rockTexture.height};
    rock->renderData = (RenderData){rock, render, RENDER_PRIORITY};
    rock->cl = cl;
    rock->hit = hit;
    rock->health = 3;
    rock->maxHealth = 3;
    rock->invulnerableTimer = 0;
    rock->healthBar = new Bar(x - 8, y - 10, rock->maxHealth, false);
    addRender(rock->renderData);
    rock->gameobject =
        createGameObject("rock", rock, getRockCollider, update);
    return rock;
}
