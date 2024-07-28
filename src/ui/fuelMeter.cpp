#include "fuelMeter.h"
#include "raylib.h"
#include "render.h"
#include "utils.h"

static Texture2D fuelBase;
static Texture2D fuelBar;
static Texture2D lightGreen;
static Texture2D lightYellow;

static i32 fuelBarWidth;
static i32 fuelBarHeight;

static bool loaded = false;

static const v2 lightPos = {85, 8};
static const v2 barPos = {10, 26};
static const i32 RENDER_PRIORITY = 20;

static void render(void* obj) {
    // NOTE:  base texture contains the background of the progress bar
    FuelMeter* fm = (FuelMeter*)obj;
    DrawTexture(fuelBase, fm->rect.x, fm->rect.y, WHITE);

    fm->currFuel = lerp(fm->currFuel, fm->fuel, 0.1f);

    f32 scale = fm->currFuel / fm->maxFuel;
    Rect n = (Rect){barPos.x, barPos.y, fuelBar.width, fuelBar.height};
    Rect src = {0, 0, fuelBarWidth * scale, fuelBarHeight};
    n.width *= scale;

    DrawTexture(fuelBase, fm->rect.x, fm->rect.y, WHITE);
    DrawTexturePro(fuelBar, src, n, (v2){0, 0}, 0, WHITE);
    Texture2D light;
    if (fm->currFuel > fm->maxFuel * 0.99) {
        light = lightGreen;
    } else {
        light = lightYellow;
    }

    DrawTexture(light, fm->rect.x + lightPos.x, fm->rect.y + lightPos.y,
                WHITE);
}

static void init(void) {
    if (!loaded) {
        fuelBase = LoadTexture("assets/images/fuelMeter/fuelMeterBase.png");
        fuelBar = LoadTexture("assets/images/fuelMeter/barFull.png");
        lightGreen = LoadTexture("assets/images/fuelMeter/lightGreen.png");
        lightYellow =
            LoadTexture("assets/images/fuelMeter/lightYellow.png");

        fuelBarWidth = fuelBar.width;
        fuelBarHeight = fuelBar.height;

        loaded = true;
    }
}

FuelMeter* FuelMeterCreate(f32 x, f32 y, f32 max) {
    init();
    FuelMeter* fuelMeter = (FuelMeter*)malloc(sizeof(FuelMeter));

    fuelMeter->fuel = 0;
    fuelMeter->maxFuel = max;
    fuelMeter->rect = (Rect){x, y, 100, 20};

    addRender((RenderData){fuelMeter, render, RENDER_PRIORITY});
    return fuelMeter;
}
