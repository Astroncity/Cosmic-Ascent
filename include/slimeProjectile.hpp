#pragma once
#include "defs.h"
#include "render.h"

typedef struct SlimeProjectile {
    v2 pos;
    f32 direction;
    f32 speed;
    RenderData renderData;
} SlimeProjectile;

SlimeProjectile* SlimeProjectileCreate(v2 start, f32 dir, f32 speed);
