#pragma once
#include "bar.h"
#include "defs.h"

typedef struct Slime {
    f32 frame;
    Rect rect;
    i32 maxHealth;
    i32 health;
    Bar* healthBar;
} Slime;

Slime* SlimeCreate();
