#include "player.h"
#include "raylib.h"

static const f32 PLAYER_SPEED = 50;
static const f32 PLAYER_SCALE = 1;
static const char* PLAYER_SPRITE = "assets/images/ghosty.png";

static Texture2D playerTextures[4];
static bool loadedTextures = false;

static u8 playerDirection = 0;

static void render(void* p) {
    Texture2D texture = playerTextures[playerDirection];
    Player* player = (Player*)p;
    Rectangle src = {0, 0, texture.width, texture.height};
    Vector2 origin = {player->rect.width / 2.0, player->rect.height / 2.0};

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

static void update(void* p) {
    Player* player = (Player*)p;
    f32 delta = GetFrameTime();
    f32 localSpeed = player->speed * delta;
    player->weaponData.use(player->weaponData.weapon);
    if (IsKeyDown(KEY_LEFT_SHIFT)) {
        localSpeed *= 2;
    }
    if (IsKeyDown(KEY_W)) {
        player->rect.y -= localSpeed;
        playerDirection = 1;
    }
    if (IsKeyDown(KEY_S)) {
        player->rect.y += localSpeed;
        playerDirection = 0;
    }
    if (IsKeyDown(KEY_A)) {
        player->rect.x -= localSpeed;
        playerDirection = 2;
    }
    if (IsKeyDown(KEY_D)) {
        player->rect.x += localSpeed;
        playerDirection = 3;
    }
}

Player* PlayerCreate(f32 x, f32 y) {
    loadTextures();
    Player* player = malloc(sizeof(Player));
    player->speed = PLAYER_SPEED;
    i32 w = playerTextures[0].width * PLAYER_SCALE;
    i32 h = playerTextures[0].height * PLAYER_SCALE;
    player->rect = (Rectangle){x, y, w, h};
    player->render = render;
    player->update = update;
    return player;
}
