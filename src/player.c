#include "player.h"
#include "globals.h"
#include "raylib.h"
#include "task.h"
#include <stdio.h>

static const f32 PLAYER_SPEED = 50;
static const f32 PLAYER_DASH_SPEED = 300;
static const f32 PLAYER_SCALE = 1;

static Texture2D playerTextures[4];
static bool loadedTextures = false;
static bool dashing = false;
static f32 dashTimer = 0;
static f32 immunityTimer = 0;

static u8 playerDirection = 0;

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
    .playerDirection = &playerDirection,
    .dashing = &dashing,
};

Rect getPlayerCollider(Player* plr) {
    Rect col = plr->rect;
    col.x -= plr->rect.width / 2;
    col.y -= plr->rect.height / 2;
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

static void render(void* p) {
    Texture2D texture = playerTextures[playerDirection];
    Player* player = (Player*)p;
    Rect src = {0, 0, texture.width, texture.height};
    v2 origin = {player->rect.width / 2.0, player->rect.height / 2.0};

    DrawTexturePro(texture, src, player->rect, origin, 0, WHITE);
}

static void loadTextures() {
    if (!loadedTextures) {
        playerTextures[0] = LoadTexture("assets/images/playerDown.png");
        playerTextures[1] = LoadTexture("assets/images/playerUp.png");
        playerTextures[2] = LoadTexture("assets/images/playerLeft.png");
        playerTextures[3] = LoadTexture("assets/images/playerRight.png");
    }
}

static void onLevelup(Player* plr) {
    plr->level++;
    plr->exp = 0;
    plr->expThreshold = getExpThreshold(plr->level);
}

static void movePlayer(Player* plr) {
    f32 delta = GetFrameTime();
    f32 localSpeed = plr->speed * delta;
    if (IsKeyDown(KEY_LEFT_SHIFT)) {
        localSpeed *= 2;
    }
    if (IsKeyDown(KEY_W)) {
        plr->rect.y -= localSpeed;
        playerDirection = 1;
    }
    if (IsKeyDown(KEY_S)) {
        plr->rect.y += localSpeed;
        playerDirection = 0;
    }
    if (IsKeyDown(KEY_A)) {
        plr->rect.x -= localSpeed;
        playerDirection = 2;
    }
    if (IsKeyDown(KEY_D)) {
        plr->rect.x += localSpeed;
        playerDirection = 3;
    }

    if (IsKeyPressed(KEY_F3)) {
        plr->health -= 10;
    }
}

static void update(void* p) {
    Player* plr = (Player*)p;
    if (!dashing) movePlayer(plr);

    if (plr->canDash && !dashing && IsKeyPressed(KEY_SPACE) &&
        GetTime() > dashTimer + plr->dashCooldown) {

        dashTimer = GetTime();
        dashing = true;
        dashData.start = GetTime();
        dashData.player = plr;
        createTask(&dashData, dashTask);
    }

    plr->weaponData.use(plr->weaponData.weapon);

    if (plr->exp >= plr->expThreshold) {
        onLevelup(plr);
    }

    if (immunityTimer > 0) {
        immunityTimer -= GetFrameTime();
    }
}

void damagePlayer(f32 dmg) {
    if (immunityTimer > 0) return;
    player->health -= dmg;
    immunityTimer = 0.1;
}

Player* PlayerCreate(f32 x, f32 y) {
    loadTextures();
    Player* player = malloc(sizeof(Player));
    player->speed = PLAYER_SPEED;
    i32 w = playerTextures[0].width * PLAYER_SCALE;
    i32 h = playerTextures[0].height * PLAYER_SCALE;
    player->rect = (Rect){x, y, w, h};
    player->render = render;
    player->update = update;
    player->maxHealth = 100;
    player->health = player->maxHealth;
    player->expThreshold = getExpThreshold(0);
    player->exp = 0;
    player->level = 0;
    player->canDash = false;
    player->dashCooldown = 5.0;
    return player;
}
