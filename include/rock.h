#pragma once
#include "bar.h"
#include "defs.h"
#include "gameobject.h"
#include "raylib.h"
#include "render.h"

typedef struct Rock Rock;
typedef struct Rock {
    Texture2D texture;
    Rect rect;
    RenderData renderData;
    Color cl;
    GameObject* gameobject;
    Bar* healthBar;
    i32 health;
    i32 maxHealth;
    f32 invulnerableTimer;
    bool (*hit)(Rock* self);
} Rock;

Rock* createRock(f32 x, f32 y, Color cl);
