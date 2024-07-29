#pragma once
#include "defs.h"
#include "gameobject.hpp"

/*typedef struct SlimeProjectile {
    v2 pos;
    f32 direction;
    f32 speed;
    RenderData renderData;
} SlimeProjectile;

SlimeProjectile* SlimeProjectileCreate(v2 start, f32 dir, f32 speed);*/

class SlimeProjectile : public GameObject {
  public:
    v2 pos;
    f32 direction;
    f32 speed;

    SlimeProjectile(v2 start, f32 dir, f32 speed);
    void update() override;
    void render() override;
    void destroy() override;
    Rect getCollider() override;
};
