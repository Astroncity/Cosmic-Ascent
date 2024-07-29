#include "player.hpp"
#include "gameobject.hpp"
#include "raylib.h"
#include "sword.hpp"
#include "task.h"
#include <stdio.h>

static const f32 PLAYER_SPEED = 50;
static const f32 PLAYER_DASH_SPEED = 300;
static const f32 PLAYER_SCALE = 1;

static Texture2D playerTextures[4];
static bool loadedTextures = false;

static i32 getExpThreshold(i32 level) { return level * level + 25; }

typedef struct DashData {
    f32 start;
    f32 duration;
    Player* player;
    u8* playerDirection;
    bool* dashing;
} DashData;

static DashData dashData = {
    .start = 0,
    .duration = 0.20,
    .player = NULL,
    .playerDirection = NULL,
    .dashing = NULL,
};

Rect Player::getCollider() {
    Rect col = rect;
    col.x -= rect.width / 2;
    col.y -= rect.height / 2;
    return col;
}

static void dashTask(TASK_PARAMS) {
    DashData* data = (DashData*)taskData;
    bool ended = GetTime() >= data->start + data->duration;
    if (ended) {
        *data->dashing = false;
        task->setForDeletion = true;
        return;
    }

    switch (*data->playerDirection) {
    case 0:
        data->player->rect.y += PLAYER_DASH_SPEED * GetFrameTime();
        break;
    case 1:
        data->player->rect.y -= PLAYER_DASH_SPEED * GetFrameTime();
        break;
    case 2:
        data->player->rect.x -= PLAYER_DASH_SPEED * GetFrameTime();
        break;
    case 3:
        data->player->rect.x += PLAYER_DASH_SPEED * GetFrameTime();
        break;
    }
}

void Player::render() {
    Texture2D texture = playerTextures[playerDirection];
    Rect src = {0, 0, (f32)texture.width, (f32)texture.height};
    v2 origin = {rect.width / 2, rect.height};

    DrawTexturePro(texture, src, rect, origin, 0, WHITE);
    weaponData.render(weaponData.weapon);
}

static void loadTextures() {
    if (!loadedTextures) {
        playerTextures[0] =
            LoadTexture("assets/images/player/playerDown.png");
        playerTextures[1] =
            LoadTexture("assets/images/player/playerUp.png");
        playerTextures[2] =
            LoadTexture("assets/images/player/playerLeft.png");
        playerTextures[3] =
            LoadTexture("assets/images/player/playerRight.png");
    }
    loadedTextures = true;
}

void Player::onLevelup() {
    level++;
    exp = 0;
    expThreshold = getExpThreshold(level);
}

void Player::move() {
    f32 delta = GetFrameTime();
    f32 localSpeed = speed * delta;
    if (IsKeyDown(KEY_LEFT_SHIFT)) {
        localSpeed *= 2;
    }
    if (IsKeyDown(KEY_W)) {
        rect.y -= localSpeed;
        playerDirection = 1;
    }
    if (IsKeyDown(KEY_S)) {
        rect.y += localSpeed;
        playerDirection = 0;
    }
    if (IsKeyDown(KEY_A)) {
        rect.x -= localSpeed;
        playerDirection = 2;
    }
    if (IsKeyDown(KEY_D)) {
        rect.x += localSpeed;
        playerDirection = 3;
    }

    if (IsKeyPressed(KEY_F3)) {
        health -= 10;
    }
}

void Player::update() {
    if (!dashing) move();

    if (canDash && !dashing && IsKeyPressed(KEY_SPACE) &&
        GetTime() > dashTimer + dashCooldown) {

        dashTimer = GetTime();
        dashing = true;
        dashData.start = GetTime();
        dashData.player = this;
        dashData.playerDirection = &playerDirection;
        dashData.dashing = &dashing;
        createTask("player dash", &dashData, dashTask);
    }

    weaponData.use(weaponData.weapon);

    if (exp >= expThreshold) {
        onLevelup();
    }

    if (immunityTimer > 0) {
        immunityTimer -= GetFrameTime();
    }
}

void Player::damage(f32 dmg) {
    if (immunityTimer > 0) return;
    health -= dmg;
    immunityTimer = 0.1;
}

void Player::destroy() { return; }

Player::Player(f32 x, f32 y) : GameObject("player", 2) {
    loadTextures();
    speed = PLAYER_SPEED;
    i32 w = playerTextures[0].width * PLAYER_SCALE;
    i32 h = playerTextures[0].height * PLAYER_SCALE;
    rect = {x, y, w, h};
    maxHealth = 100;
    health = maxHealth;
    expThreshold = getExpThreshold(0);
    exp = 0;
    level = 0;
    canDash = false;
    dashCooldown = 5.0;
    dashTimer = 0;
    dashing = false;
    playerDirection = 0;
    immunityTimer = 0;
}
