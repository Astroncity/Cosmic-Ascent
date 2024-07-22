#include "bar.h"
#include "utils.h"

static Texture2D bigBarTexture;
static Texture2D bigBarBackground;
static Texture2D smallBarTexture;
static Texture2D smallBarBackground;
static bool loaded = false;

Bar* BarCreate(f32 x, f32 y, f32 maxValue, bool large) {
    Bar* bar = malloc(sizeof(Bar));
    Texture2D barTexture;
    Texture2D background;

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

    bar->barTexture = barTexture;
    bar->background = background;
    bar->rect = (Rect){x, y, background.width, background.height};
    bar->value = maxValue;
    bar->oldValue = maxValue;
    bar->maxValue = maxValue;
    bar->currValue = maxValue;
    return bar;
}

void BarRender(Bar* bar, Color tint, bool drawBackground) {
    bar->currValue = lerp(bar->currValue, bar->value, 0.1f);

    f32 scale = bar->currValue / bar->maxValue;
    Rect new = bar->rect;
    Rect src = {0, 0, bar->barTexture.width * scale,
                bar->barTexture.height};
    new.width *= scale;
    if (drawBackground) {
        DrawTexturePro(
            bar->background,
            (Rect){0, 0, bar->background.width, bar->background.height},
            bar->rect, (v2){0, 0}, 0, WHITE);
    }
    DrawTexturePro(bar->barTexture, src, new, (v2){0, 0}, 0, tint);
}
