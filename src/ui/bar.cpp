#include "bar.hpp"
#include "utils.h"

static Texture2D bigBarTexture;
static Texture2D bigBarBackground;
static Texture2D smallBarTexture;
static Texture2D smallBarBackground;
static bool loaded = false;

Bar::Bar(f32 x, f32 y, f32 maxValue, bool large) {
    if (!loaded) {
        bigBarTexture = LoadTexture("assets/images/barBot.png");
        bigBarBackground = LoadTexture("assets/images/barBot.png");
        smallBarTexture = LoadTexture("assets/images/smallBarTop.png");
        smallBarBackground =
            LoadTexture("assets/images/smallBarBackground.png");
        loaded = true;
    }

    if (large) {
        barTexture = bigBarTexture;
        background = bigBarBackground;
    } else {
        barTexture = smallBarTexture;
        background = smallBarBackground;
    }

    rect = (Rect){x, y, background.width, background.height};
    value = maxValue;
    oldValue = maxValue;
    this->maxValue = maxValue;
    currValue = maxValue;
}

void Bar::render(Color tint, bool drawBackground) {
    currValue = lerp(currValue, value, 0.1f);

    f32 scale = currValue / maxValue;
    Rect n = rect;
    Rect src = {0, 0, barTexture.width * scale, barTexture.height};
    n.width *= scale;
    if (drawBackground) {
        DrawTexturePro(background,
                       (Rect){0, 0, background.width, background.height},
                       rect, (v2){0, 0}, 0, WHITE);
    }
    DrawTexturePro(barTexture, src, n, (v2){0, 0}, 0, tint);
}

void Bar::updatePos(f32 x, f32 y) {
    rect.x = x;
    rect.y = y;
}
