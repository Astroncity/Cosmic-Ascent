#pragma once
#include "defs.h"
#include "player.h"

typedef struct Slime {
    f32 frame;
    Rect rect;
} Slime;

Slime* SlimeCreate();
void SlimeInit(Player* plr, v2* mousePos);
