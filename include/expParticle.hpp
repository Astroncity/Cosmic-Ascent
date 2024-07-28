#pragma once
#include "defs.h"

/*typedef struct ExpParticle {
    v2 pos;
    Color cl;
    i32 value;
} ExpParticle;

void ExpParticleCreate(v2 pos, Color cl, i32 value);
void ExpParticleBatchCreate(v2 pos, Color cl, i32 value, i32 count);
void ExpParticleUpdateAll();
void ExpParticleDrawAll();*/

class ExpParticle {
  public:
    v2 pos;
    Color cl;
    i32 value;
    ExpParticle();
    ExpParticle(v2 pos, Color cl, i32 value);
    static void batchCreate(v2 pos, Color cl, i32 value, i32 count);
    static void updateAll();
    static void drawAll();
};
