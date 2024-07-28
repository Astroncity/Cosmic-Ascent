#include "expParticle.hpp"
#include "globals.h"
#include "player.h"
#include "raylib.h"
#include <list>

#define EXP_PARTICLE_RAND 50
#define EXP_PARTICLE_RAD 3

static std::list<ExpParticle*> expParticles;

bool collides(ExpParticle* p) {
    if (CheckCollisionCircleRec(p->pos, EXP_PARTICLE_RAD,
                                getPlayerCollider(player))) {
        return true;
    }
    return false;
}

ExpParticle::ExpParticle(v2 pos, Color cl, i32 value) {
    this->pos = pos;
    this->cl = cl;
    this->value = value;
    expParticles.push_front(this);
}
void ExpParticle::batchCreate(v2 pos, Color cl, i32 value, i32 count) {
    for (i32 i = 0; i < count; i++) {
        v2 randPos;
        f32 randx =
            (f32)GetRandomValue(0, EXP_PARTICLE_RAND * 100) / 100.0f;
        f32 randy =
            (f32)GetRandomValue(0, EXP_PARTICLE_RAND * 100) / 100.0f;
        randPos.x = pos.x + randx;
        randPos.y = pos.y + randy;
        new ExpParticle(randPos, cl, value);
    }
}

void ExpParticle::updateAll() {
    for (auto it = expParticles.begin(); it != expParticles.end();) {
        ExpParticle* p = *it;

        if (collides(p)) {
            player->exp += p->value;
            it = expParticles.erase(it);
            delete p;
        } else {
            it++;
        }
    }
}

void ExpParticle::drawAll() {
    for (auto it = expParticles.begin(); it != expParticles.end(); it++) {
        ExpParticle* p = *it;
        DrawCircleV(p->pos, EXP_PARTICLE_RAD, p->cl);
    }
}
