#pragma once
#include "defs.h"
#include "raylib.h"
#include "render.h"

typedef struct Rock {
    Texture2D texture;
    Rectangle rect;
    RenderData renderData;
    Color cl;
} Rock;

Rock* createRock(f32 x, f32 y, Color cl);
