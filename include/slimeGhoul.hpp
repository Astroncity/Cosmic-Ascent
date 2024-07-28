#pragma once
#include "bar.hpp"
#include "defs.h"
#include "gameobject.h"

typedef struct SlimeGhoul {
    f32 frame;
    Rect rect;
    f32 projectileTimer;
    i32 maxHealth;
    i32 health;
    Bar* healthBar;
    GameObject* gameobject;
} SlimeGhoul;

SlimeGhoul* SlimeGhoulCreate(v2 pos);
