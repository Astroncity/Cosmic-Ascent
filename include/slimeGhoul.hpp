#pragma once
#include "bar.hpp"
#include "defs.h"
#include "gameobject.hpp"

/*typedef struct SlimeGhoul {
    f32 frame;
    Rect rect;
    f32 projectileTimer;
    i32 maxHealth;
    i32 health;
    Bar* healthBar;
    GameObject* gameobject;
} SlimeGhoul;

SlimeGhoul* SlimeGhoulCreate(v2 pos);*/

class SlimeGhoul : public GameObject {
  public:
    f32 frame;
    Rect rect;
    f32 projectileTimer;
    i32 maxHealth;
    i32 health;
    Bar* healthBar;

    SlimeGhoul(v2 pos);

    Rect getCollider() override;
    void update() override;
    void destroy() override;
    void render() override;

  private:
    void handleCollision();
};
