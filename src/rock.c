#include "rock.h"
#include "gameobject.h"
#include "raylib.h"
#include <stdio.h>

const char* TEXTURE_PATH = "assets/images/rock.png";
const i32 RENDER_PRIORITY = 3;
Texture2D rockTexture;

Color shadowTint = {0, 0, 0, 150};

static Rectangle getRockCollider(GameObject* self) {
    Rock* rock = (Rock*)self->obj;
    Rectangle cpy = rock->rect;
    cpy.x -= rock->rect.width / 2;
    cpy.y -= rock->rect.height / 2;
    return cpy;
}

static void drawCol(Rock* rock) {
    Rectangle cpy = rock->rect;
    cpy.x -= rock->rect.width / 2;
    cpy.y -= rock->rect.height / 2;
    DrawRectangleLines(cpy.x, cpy.y, cpy.width, cpy.height, GREEN);
}

static void render(void* rockP) {
    Rock* rock = (Rock*)rockP;

    if (rock->invulnerableTimer > 0) {
        rock->invulnerableTimer -= GetFrameTime();
    }

    Rectangle dest = rock->rect;

    Rectangle src = {0, 0, rock->rect.width, rock->rect.height};
    v2 org = {dest.width / 2, dest.height / 2};
    // shadow
    // DrawEllipse(rock->rect.x, rock->rect.y + 7, dest.width / 2,
    // dest.height / 4, shadowTint);
    Rectangle shadowDest = dest;
    shadowDest.y += 3;
    DrawTexturePro(rock->texture, src, shadowDest, org, 0, shadowTint);

    DrawTexturePro(rock->texture, src, dest, org, 0, rock->cl);
    BarRender(rock->healthBar, rock->cl);
    if (false) drawCol(rock);
}

static void destroy(Rock* self) {
    removeRender(self->renderData);
    removeGameObject(self->gameobject);
    free(self);
}

void hit(Rock* self) {
    if (self->invulnerableTimer > 0) {
        return;
    }
    self->invulnerableTimer = 0.25;
    self->health -= 1;
    self->healthBar->value = self->health;
    if (self->health <= 0) {
        destroy(self);
    }
}

Rock* createRock(f32 x, f32 y, Color cl) {
    if (rockTexture.id == 0) {
        Image temp = LoadImage(TEXTURE_PATH);
        rockTexture = LoadTextureFromImage(temp);
    }

    Rock* rock = malloc(sizeof(Rock));
    rock->texture = rockTexture;
    rock->rect = (Rectangle){x, y, rockTexture.width, rockTexture.height};
    rock->renderData = (RenderData){rock, render, RENDER_PRIORITY};
    rock->cl = cl;
    rock->hit = hit;
    rock->health = 3;
    rock->maxHealth = 3;
    rock->healthBar = BarCreate(x - 8, y - 10, rock->maxHealth, false);
    printf("reached\n");
    addRender(rock->renderData);
    rock->gameobject = createGameObject("rock", rock, getRockCollider);
    return rock;
}
