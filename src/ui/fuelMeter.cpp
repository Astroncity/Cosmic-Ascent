#include "fuelMeter.hpp"
#include "raylib.h"
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

void FuelMeter::render() {
    // NOTE:  base texture contains the background of the progress bar
    DrawTexture(fuelBase, rect.x, rect.y, WHITE);

    currFuel = lerp(currFuel, fuel, 0.1f);

    f32 scale = currFuel / maxFuel;
    Rect n = (Rect){barPos.x, barPos.y, fuelBar.width, fuelBar.height};
    Rect src = {0, 0, fuelBarWidth * scale, fuelBarHeight};
    n.width *= scale;

    DrawTexture(fuelBase, rect.x, rect.y, WHITE);
    DrawTexturePro(fuelBar, src, n, (v2){0, 0}, 0, WHITE);
    Texture2D light;
    if (currFuel > maxFuel * 0.99) {
        light = lightGreen;
    } else {
        light = lightYellow;
    }

    DrawTexture(light, rect.x + lightPos.x, rect.y + lightPos.y, WHITE);
}

void FuelMeter::update() {
    if (fuel > maxFuel) {
        fuel = maxFuel;
    }
}

void FuelMeter::destroy() {
    UnloadTexture(fuelBase);
    UnloadTexture(fuelBar);
    UnloadTexture(lightGreen);
    UnloadTexture(lightYellow);
    loaded = false;
    markedForDeletion = true;
}

Rect FuelMeter::getCollider() { return rect; }

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

FuelMeter::FuelMeter(f32 x, f32 y, f32 max) : GameObject("fuelMeter", 25) {
    init();
    fuel = 0;
    maxFuel = max;
    rect = (Rect){x, y, 100, 20};
}
