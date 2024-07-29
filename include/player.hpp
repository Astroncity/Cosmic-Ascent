#pragma once
#include "defs.h"
#include "gameobject.hpp"
#include "sword.hpp"

/*typedef struct Player {
    Rect rect;
    f32 speed;
    i32 exp;
    i32 expThreshold;
    i32 level;
    f32 health;
    f32 maxHealth;
    bool canDash;
    f32 dashCooldown;
    void (*render)(void*);
    void (*update)(void*);
    WeaponData weaponData;
} Player;

Player* PlayerCreate(f32 x, f32 y);
Rect getPlayerCollider(Player* plr);
void damagePlayer(f32 dmg);*/

class Player : public GameObject {
  public:
    Rect rect;
    f32 speed;
    i32 exp;
    i32 expThreshold;
    i32 level;
    f32 health;
    f32 maxHealth;
    bool canDash;
    bool dashing;
    f32 dashTimer;
    f32 immunityTimer;
    u8 playerDirection;
    f32 dashCooldown;
    WeaponData weaponData;

    Player(f32 x, f32 y);
    Rect getCollider() override;
    void update() override;
    void destroy() override;
    void render() override;
    void damage(f32 dmg);

  private:
    void move();
    void onLevelup();
};
