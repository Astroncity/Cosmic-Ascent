#pragma once
#include "bar.hpp"
#include "defs.h"
#include "gameobject.hpp"

/*typedef struct Slime {
    f32 frame;
    Rect rect;
    i32 maxHealth;
    i32 health;
    Bar* healthBar;
    GameObject* gameobject;
} Slime;

Slime* SlimeCreate(v2 pos);*/

class Slime : public GameObject {
  public:
    f32 frame;
    Rect rect;
    i32 maxHealth;
    i32 health;
    Bar* healthBar;

    Slime(v2 pos);

    Rect getCollider() override;
    void update() override;
    void destroy() override;

  private:
    void handleCollision();
};
