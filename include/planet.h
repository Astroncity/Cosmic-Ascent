#pragma once
#include "defs.h"
#include "raylib.h"

#define MAX(a, v) ((a) > (v) ? (a) : (v))
#define MIN(a, v) ((a) < (v) ? (a) : (v))

typedef struct Planet {
    Texture2D texture;
    Texture2D test;
    Texture2D atmosphere;
    Vector2 pos;
    i32 size;
} Planet;

Planet genPlanet(i32 imgSize, bool randomizeColors);
void drawPlanet(Planet *planet);
