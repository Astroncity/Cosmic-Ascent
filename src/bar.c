#include "bar.h"
#include "utils.h"

static Texture2D bigBarTexture;
static Texture2D bigBarBackground;
static Texture2D smallBarTexture;
static Texture2D smallBarBackground;
bool loaded = false;

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
    bar->rect = (Rectangle){x, y, background.width, background.height};
    bar->value = maxValue;
    bar->oldValue = maxValue;
    bar->maxValue = maxValue;
    bar->currValue = maxValue;
    return bar;
}

void BarRender(Bar* bar, Color tint) {
    bar->currValue = lerp(bar->currValue, bar->value, 0.1f);

    f32 scale = bar->currValue / bar->maxValue;
    Rectangle new = bar->rect;
    Rectangle src = {0, 0, bar->barTexture.width * scale,
                     bar->barTexture.height};
    new.width *= scale;

    DrawTexturePro(bar->barTexture, src, new, (v2){0, 0}, 0, tint);
}
