#pragma once
#include "defs.h"
#include "raylib.h"

#define MAX(a, v) ((a) > (v) ? (a) : (v))
#define MIN(a, v) ((a) < (v) ? (a) : (v))

typedef struct Planet {
    Texture2D texture;
    Texture2D atmosphere;
    Texture2D thumbnail;
    Texture2D thumbnailAtmosphere;
    Vector2 pos;
    i32 size;
    Color terrainColor;
    Color* palette;
} Planet;

Planet genPlanet(i32 imgSize, bool randomizeColors);
void drawPlanet(Planet* planet, f32 scale);
void drawPlanetThumbnail(Vector2 pos, Planet* planet);
Color setColorShadow(Color c, i32 shadow);
Color HSLtoRGB(f32 h, f32 s, f32 l);
void RGBtoHSL(Color color, f32* h, f32* s, f32* l);
Color normalizeColor(Color c);
