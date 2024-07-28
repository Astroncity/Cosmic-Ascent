#include "rock.hpp"
#include "expParticle.hpp"
#include "raylib.h"
#include "render.h"

const char* TEXTURE_PATH = "assets/images/rock.png";
const i32 RENDER_PRIORITY = 5;
const i32 ROCK_EXP = 2;
Texture2D rockTexture;
Sound rockBreakfx;
Sound rockHitfx;

Color shadowTint = {0, 0, 0, 150};

Rect Rock::getCollider() {
    Rect cpy = rect;
    cpy.x -= rect.width / 2;
    cpy.y -= rect.height / 2;
    return cpy;
}

static void drawCol(Rock* rock) {
    Rect cpy = rock->rect;
    cpy.x -= rock->rect.width / 2;
    cpy.y -= rock->rect.height / 2;
    DrawRectangleLines(cpy.x, cpy.y, cpy.width, cpy.height, GREEN);
}

void Rock::update() {
    if (invulnerableTimer > 0) {
        invulnerableTimer -= GetFrameTime();
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

void Rock::destroy() {
    if (!IsSoundReady(rockBreakfx)) {
        rockBreakfx = LoadSound("assets/sounds/rockBreakfx.wav");
        SetSoundVolume(rockBreakfx, 0.45);
    }
    PlaySound(rockBreakfx);

    ExpParticle::batchCreate((Vector2){rect.x, rect.y}, cl, ROCK_EXP, 10);

    removeRender(RenderData{(void*)this, render, RENDER_PRIORITY});
    delete healthBar;
    markedForDeletion = true;
}

bool Rock::hit() {
    if (invulnerableTimer > 0) {
        return false;
    }

    if (!IsSoundReady(rockHitfx)) {
        rockHitfx = LoadSound("assets/sounds/rockHitfx.wav");
        SetSoundVolume(rockHitfx, 0.45);
    }
    PlaySound(rockHitfx);

    invulnerableTimer = 0.1;
    health -= 1;
    healthBar->value = health;
    if (health <= 0) {
        destroy();
        return true;
    }
    return false;
}

Rock::Rock(f32 x, f32 y, Color cl) : GameObject("rock") {
    if (rockTexture.id == 0) {
        Image temp = LoadImage(TEXTURE_PATH);
        rockTexture = LoadTextureFromImage(temp);
    }

    texture = rockTexture;
    rect = (Rect){x, y, rockTexture.width, rockTexture.height};
    this->cl = cl;
    health = 3;
    maxHealth = 3;
    invulnerableTimer = 0;
    healthBar = new Bar(x - 8, y - 10, maxHealth, false);
    addRender(RenderData{this, render, RENDER_PRIORITY});
}
