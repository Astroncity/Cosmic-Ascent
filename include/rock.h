#pragma once
#include "defs.h"
#include "gameobject.h"
#include "raylib.h"
#include "render.h"

typedef struct Rock Rock;
typedef struct Rock {
    Texture2D texture;
    Rectangle rect;
    RenderData renderData;
    Color cl;
    GameObject* gameobject;
    void (*hit)(Rock* self);
} Rock;

Rock* createRock(f32 x, f32 y, Color cl);
