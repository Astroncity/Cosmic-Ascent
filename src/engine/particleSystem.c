#include "particleSystem.h"

static void emit(ParticleSystem* ps) {
    Particle* p = malloc(sizeof(Particle));
    p->pos = ps->pos;
    p->vel = (v2){(f32)rand() / RAND_MAX * 2 - 1,
                  (f32)rand() / RAND_MAX * 2 - 1};
    p->life = 0;
    p->maxLife = ps->lifeTime;
    p->size = ps->size;
    p->color = (Color){(u8)(rand() % 255), (u8)(rand() % 255),
                       (u8)(rand() % 255), 255};
    ParticleNode* node = malloc(sizeof(ParticleNode));
    node->particle = p;
    node->next = ps->head;
    ps->head = node;
}

void updateParticleSystem(ParticleSystem* ps) {
    ps->timer += GetFrameTime();
    if (ps->timer > ps->rate) {
        emit(ps);
        ps->timer = 0;
    }

    ParticleNode* prev = NULL;
    ParticleNode* curr = ps->head;

    while (curr != NULL) {
        Particle* part = curr->particle;

        if (part->life > part->maxLife) {
            if (prev == NULL) {
                ps->head = curr->next;
                free(part);
                free(curr);
                curr = ps->head;
                continue;
            } else {
                prev->next = curr->next;
                free(part);
                free(curr);
                curr = prev->next;
                continue;
            }

        } else {
            part->pos.x += part->vel.x;
            part->pos.y += part->vel.y;
            part->life += GetFrameTime();
            prev = curr;
            curr = curr->next;
        }
    }
}

void drawParticleSystem(ParticleSystem* ps) {
    DrawCircleV(ps->pos, 5, RED);
    ParticleNode* curr = ps->head;
    while (curr != NULL) {
        Particle* part = curr->particle;
        DrawRectangleV(part->pos, (v2){part->size, part->size},
                       part->color);
        curr = curr->next;
    }
}

ParticleSystem* createParticleSystem(v2 pos, f32 rate, f32 lifeTime,
                                     f32 size, f32 speed) {
    ParticleSystem* ps = malloc(sizeof(ParticleSystem));
    ps->head = NULL;
    ps->pos = pos;
    ps->rate = rate;
    ps->lifeTime = lifeTime;
    ps->size = size;
    ps->speed = speed;
    ps->timer = 0;
    return ps;
}
