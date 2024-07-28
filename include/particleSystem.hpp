#pragma once
#include "defs.h"
#include "gameobject.hpp"

typedef enum ParticleEmissionType {
    PARTICLE_EMIT_RADIAL,
    PARTICLE_EMIT_LINEAR,
} ParticleEmissionType;

/*typedef struct Particle {
    v2 pos;
    v2 vel;
    f32 life;
    f32 maxLife;
    f32 size;
    Color color;
} Particle;

typedef struct ParticleSystem {
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
void drawParticleSystem(ParticleSystem* ps);*/

class Particle {
  public:
    v2 pos;
    v2 vel;
    f32 life;
    f32 maxLife;
    f32 size;
    Color color;

    Particle(v2 pos, v2 vel, f32 life, f32 size, Color color) {
        this->pos = pos;
        this->vel = vel;
        this->life = 0;
        this->maxLife = life;
        this->size = size;
        this->color = color;
    }
};

class ParticleSystem : public GameObject {
  public:
    v2 pos;
    f32 timer;
    f32 rate;
    f32 lifeTime;
    f32 size;
    f32 speed;

    static std::list<Particle*> particles;

    ParticleSystem(v2 pos, f32 rate, f32 lifeTime, f32 size, f32 speed);
    void update() override;
    Rect getCollider() override;
    void destroy() override;
    void draw();
    void emit();
    // void destroy() override;
};
