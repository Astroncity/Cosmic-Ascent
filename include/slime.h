#pragma once
#include "defs.h"
#include "player.h"

typedef struct Slime {
    f32 frame;
    Rect rect;
} Slime;

Slime* SlimeCreate();
