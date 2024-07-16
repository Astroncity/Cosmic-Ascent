#pragma once
#include "defs.h"

typedef struct ExpParticle {
    v2 pos;
    Color cl;
    i32 value;
} ExpParticle;

void ExpParticleCreate(v2 pos, Color cl, i32 value);
void ExpParticleBatchCreate(v2 pos, Color cl, i32 value, i32 count);
void ExpParticleUpdateAll();
void ExpParticleDrawAll();
