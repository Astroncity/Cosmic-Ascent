#pragma once
#include "defs.h"

typedef struct SlimeGhoul {
    f32 frame;
    Rect rect;
    f32 projectileTimer;
} SlimeGhoul;

SlimeGhoul* SlimeGhoulCreate();
