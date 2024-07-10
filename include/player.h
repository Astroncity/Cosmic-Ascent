#pragma once
#include "defs.h"
#include "raylib.h"
#include "sword.h"

typedef struct Player {
    Rectangle rect;
    f32 speed;
    i32 exp;
    i32 level;
    void (*render)(void*);
    void (*update)(void*);
    WeaponData weaponData;
} Player;

Player* PlayerCreate(f32 x, f32 y);
