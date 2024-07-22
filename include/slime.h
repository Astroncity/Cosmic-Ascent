#pragma once
#include "bar.h"
#include "defs.h"
#include "gameobject.h"

typedef struct Slime {
    f32 frame;
    Rect rect;
    i32 maxHealth;
    i32 health;
    Bar* healthBar;
    GameObject* gameobject;
} Slime;

Slime* SlimeCreate(v2 pos);
