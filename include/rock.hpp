#pragma once
#include "bar.hpp"
#include "defs.h"
#include "gameobject.hpp"
#include "raylib.h"

/*typedef struct Rock Rock;
typedef struct Rock {
    Texture2D texture;
    Rect rect;
    RenderData renderData;
    Color cl;
    GameObject* gameobject;
    Bar* healthBar;
    i32 health;
    i32 maxHealth;
    f32 invulnerableTimer;
    bool (*hit)(Rock* self);
} Rock;

Rock* createRock(f32 x, f32 y, Color cl);*/

class Rock : public GameObject {
  public:
    Texture2D texture;
    Rect rect;
    Color cl;
    Bar* healthBar;
    i32 health;
    i32 maxHealth;
    f32 invulnerableTimer;
    bool hit();

    Rock(f32 x, f32 y, Color cl);

    void update() override;
    void destroy() override;
    Rect getCollider() override;
    void render() override;

  private:
    void drawCol();
};
