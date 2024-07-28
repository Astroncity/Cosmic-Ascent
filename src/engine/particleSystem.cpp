#include "particleSystem.hpp"

std::list<Particle*> ParticleSystem::particles;

void ParticleSystem::emit() {
    v2 vel = {(f32)rand() / RAND_MAX * 2 - 1,
              (f32)rand() / RAND_MAX * 2 - 1};
    Color color = (Color){(u8)(rand() % 255), (u8)(rand() % 255),
                          (u8)(rand() % 255), 255};

    Particle* part = new Particle(pos, vel, lifeTime, size, color);
    particles.push_back(part);
}

void ParticleSystem::update() {
    timer += GetFrameTime();
    if (timer > rate) {
        emit();
        timer = 0;
    }

    for (auto it = particles.begin(); it != particles.end(); it++) {
        Particle* part = *it;

        part->pos.x += part->vel.x;
        part->pos.y += part->vel.y;
        part->life += GetFrameTime();

        if (part->life > part->maxLife) {
            it = particles.erase(it);
            delete part;
        }
    }
}

void ParticleSystem::draw() {
    // DrawCircleV(ps->pos, 5, RED);
    for (auto it = particles.begin(); it != particles.end(); it++) {
        Particle* part = *it;
        DrawRectangleV(part->pos, (v2){part->size, part->size},
                       part->color);
    }
}

ParticleSystem::ParticleSystem(v2 pos, f32 rate, f32 lifeTime, f32 size,
                               f32 speed)
    : GameObject("particleSystem") {
    this->pos = pos;
    this->rate = 1 / rate;
    this->lifeTime = lifeTime;
    this->size = size;
    this->speed = speed;
    this->timer = 0;
}

void ParticleSystem::destroy() { return; }
Rect ParticleSystem::getCollider() { return {}; }
