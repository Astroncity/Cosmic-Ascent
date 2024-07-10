#include "rock.h"
#include "raylib.h"
#include <stdio.h>

const char* TEXTURE_PATH = "assets/images/rock.png";
const i32 RENDER_PRIORITY = 3;
Texture2D rockTexture;

Color shadowTint = {0, 0, 0, 150};

static void render(void* rockP) {
    Rock* rock = (Rock*)rockP;

    Rectangle dest = rock->rect;

    Rectangle src = {0, 0, rock->rect.width, rock->rect.height};
    Vector2 org = {dest.width / 2, dest.height / 2};
    // shadow
    // DrawEllipse(rock->rect.x, rock->rect.y + 7, dest.width / 2, dest.height /
    // 4, shadowTint);
    Rectangle shadowDest = dest;
    shadowDest.y += 3;
    DrawTexturePro(rock->texture, src, shadowDest, org, 0, shadowTint);

    DrawTexturePro(rock->texture, src, dest, org, 0, rock->cl);
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
    printf("reached\n");
    addRender(rock->renderData);
    return rock;
}
