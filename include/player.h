#pragma once
#include "defs.h"
#include "sword.hpp"

typedef struct Player {
    Rect rect;
    f32 speed;
    i32 exp;
    i32 expThreshold;
    i32 level;
    f32 health;
    f32 maxHealth;
    bool canDash;
    f32 dashCooldown;
    void (*render)(void*);
    void (*update)(void*);
    WeaponData weaponData;
} Player;

Player* PlayerCreate(f32 x, f32 y);
Rect getPlayerCollider(Player* plr);
void damagePlayer(f32 dmg);
