#pragma once
#include "raylib.h"

typedef struct Player Player;

typedef struct WeaponData {
    void* weapon;
    void (*use)(void*);
} WeaponData;

typedef struct Sword {
    Rectangle rect;
    Texture2D texture;
    Color cl;
    void (*render)(void*);
    void (*use)(void*);
    Player* owner;
    Vector2* mouse;
} Sword;

Sword* createSword(Player* owner, Vector2* mouse, Color cl);
