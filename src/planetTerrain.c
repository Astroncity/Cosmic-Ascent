#include "planetTerrain.h"
#include "defs.h"
#include "planet.h"
#include "raylib.h"
#include "rock.h"
#include <stdlib.h>

const i32 imgSize = 480;
const i32 OBJ_COUNT = 20;
const i32 MAP_SIZE = LEVEL_SCALE * imgSize;

Color satShiftColor(Color color, f32 shift) {
    f32 h, s, l;
    RGBtoHSL(color, &h, &s, &l);
    s += shift;
    return HSLtoRGB(h, s, l);
}

static void genObj(Color planetColor) {
    planetColor.a = 255;
    for (i32 i = 0; i < OBJ_COUNT; i++) {
        i32 x = GetRandomValue(50, screenWidth - 50);
        i32 y = GetRandomValue(50, screenHeight - 50);

        createRock(x, y, planetColor);
    }
}

PlanetTerrain* genPlanetTerrain(Planet* planet) {
    i32 randomSeed = GetRandomValue(0, 1000);

    // Image dirt = GenImageWhiteNoise(imgSize, imgSize, 0.5f);
    Image dirt =
        GenImagePerlinNoise(imgSize, imgSize, randomSeed, randomSeed, 20);

    Color* dirtPixels = LoadImageColors(dirt);

    Color terrainColor = normalizeColor(planet->terrainColor);
    terrainColor = satShiftColor(terrainColor, -0.3f);

    Image solid = GenImageColor(imgSize, imgSize, terrainColor);
    Color* solidPixels = LoadImageColors(solid);

    Image final = GenImageColor(imgSize, imgSize, BLANK);

    for (usize i = 0; i < imgSize * imgSize; i++) {
        i32 x = i % imgSize;
        i32 y = i / imgSize;

        Color pixel = solidPixels[i];
        f32 rand = (f32)GetRandomValue(0, 100) / 100.0f;
        f32 val = dirtPixels[i].r * rand / 900.0;

        f32 h, s, l;
        RGBtoHSL(pixel, &h, &s, &l);
        l -= val;
        l /= 2;
        h += val / 2;
        pixel = HSLtoRGB(h, s, l);

        ImageDrawPixel(&final, x, y, pixel);
    }

    free(dirtPixels);
    free(solidPixels);

    PlanetTerrain* terrain = malloc(sizeof(PlanetTerrain));
    terrain->texture = LoadTextureFromImage(final);

    Color rockC = planet->palette[GetRandomValue(0, 4)];

    genObj(rockC);

    return terrain;
}
