#pragma once
#include "defs.h"
#include "player.h"

typedef struct Slime {
    Rect rect;
    Texture2D texture;
} Slime;

Slime* SlimeCreate();
void SlimeInit(Player* plr, v2* mousePos);
