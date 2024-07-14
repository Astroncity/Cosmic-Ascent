#include "expParticle.h"
#include "player.h"
#include "raylib.h"
#include <stdio.h>
#define EXP_PARTICLE_MAX 1000
#define EXP_PARTICLE_RAND 50
#define EXP_PARTICLE_RAD 3

static i32 expParticleCount = 0;
static ExpParticle expParticles[EXP_PARTICLE_MAX];
static Player* plr;
static bool loaded = false;

void ExpParticleInit(Player* player) {
    plr = player;
    loaded = true;
}

void ExpParticleCreate(v2 pos, Color cl, i32 value) {
    if (!loaded) {
        fprintf(stderr,
                "ExpParticleCreate called before ExpParticleInit\n");
    }

    if (expParticleCount < EXP_PARTICLE_MAX) {
        ExpParticle* p = &expParticles[expParticleCount];
        p->pos = pos;
        p->cl = cl;
        p->value = value;
        expParticleCount++;
    }
}
void ExpParticleBatchCreate(v2 pos, Color cl, i32 value, i32 count) {
    for (i32 i = 0; i < count; i++) {
        v2 randPos;
        f32 randx =
            (f32)GetRandomValue(0, EXP_PARTICLE_RAND * 100) / 100.0f;
        f32 randy =
            (f32)GetRandomValue(0, EXP_PARTICLE_RAND * 100) / 100.0f;
        randPos.x = pos.x + randx;
        randPos.y = pos.y + randy;
        ExpParticleCreate(randPos, cl, value);
    }
}

void ExpParticleUpdateAll() {
    i32 i = 0;
    while (i < expParticleCount) {
        ExpParticle* p = &expParticles[i];
        if (CheckCollisionCircleRec(p->pos, EXP_PARTICLE_RAD,
                                    getPlayerCollider(plr))) {
            plr->exp += p->value;
            expParticles[i] = expParticles[expParticleCount - 1];
            expParticleCount--;
        } else {
            i++;
        }
    }
}

void ExpParticleDrawAll() {
    for (i32 i = 0; i < expParticleCount; i++) {
        ExpParticle* p = &expParticles[i];
        DrawCircleV(p->pos, EXP_PARTICLE_RAD, p->cl);
    }
}
