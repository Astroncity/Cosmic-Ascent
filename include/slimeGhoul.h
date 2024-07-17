#pragma once
#include "bar.h"
#include "defs.h"

typedef struct SlimeGhoul {
    f32 frame;
    Rect rect;
    f32 projectileTimer;
    i32 maxHealth;
    i32 health;
    Bar* healthBar;
} SlimeGhoul;

SlimeGhoul* SlimeGhoulCreate();
