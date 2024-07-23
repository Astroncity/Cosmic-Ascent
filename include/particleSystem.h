#pragma once
#include "defs.h"

typedef enum ParticleEmissionType {
    PARTICLE_EMIT_RADIAL,
    PARTICLE_EMIT_LINEAR,
} ParticleEmissionType;

typedef struct ParticleSystemDyn {
    f32 startSize;
    f32 endSize;
    f32 startSpeed;
    f32 endSpeed;
    Color startColor;
    Color endColor;
} ParticleSystemDyn;

typedef struct Particle {
    v2 pos;
    v2 vel;
    f32 life;
    f32 maxLife;
    f32 size;
    Color color;
} Particle;

typedef struct ParticleNode ParticleNode;
struct ParticleNode {
    Particle* particle;
    ParticleNode* next;
};

typedef struct ParticleSystem {
    ParticleNode* head;
    v2 pos;
    f32 timer;
    f32 rate;
    f32 lifeTime;
    f32 size;
    f32 speed;
} ParticleSystem;

ParticleSystem* createParticleSystem(v2 pos, f32 rate, f32 lifeTime,
                                     f32 size, f32 speed);
void updateParticleSystem(ParticleSystem* ps);
void drawParticleSystem(ParticleSystem* ps);
