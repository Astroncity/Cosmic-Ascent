#pragma once
#include "defs.h"
#include "raylib.h"

typedef struct Player Player;
typedef struct Sword Sword;
typedef struct SpinData {
    f32 start;
    f32 runTime;
    i8 direction;
    i8 type;
    f32 speed;
    Sword* sword;
} SpinData;

typedef struct WeaponData {
    void* weapon;
    void (*use)(void*);
    void (*render)(void*);
} WeaponData;

typedef struct Sword {
    Rect rect;
    Color cl;
    void (*render)(void*);
    void (*use)(void*);
    Player* owner;
    Vector2* mouse;

    // extra
    bool mouseControl;
    bool underRecoil;
    f32 angle;
    f32 previousAngle;
    f32 angleDelta;
    f32 invulnerableTimer;
    v2 prevPos;
    f32 len;
} Sword;

Sword* createSword(Player* owner, Vector2* mouse, Color cl);
