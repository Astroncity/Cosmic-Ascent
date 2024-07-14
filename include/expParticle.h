#pragma once
#include "defs.h"
#include "player.h"

typedef struct ExpParticle {
    v2 pos;
    Color cl;
    i32 value;
} ExpParticle;

void ExpParticleCreate(v2 pos, Color cl, i32 value);
void ExpParticleBatchCreate(v2 pos, Color cl, i32 value, i32 count);
void ExpParticleUpdateAll();
void ExpParticleDrawAll();
void ExpParticleInit(Player* player);
