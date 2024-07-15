#include "upgradeCard.h"
#include "raylib.h"
#include "textEffects.h"
#include <stdio.h>
#include <stdlib.h>

const char* UPGRADE_NAMES[] = {
    "Sword Length", "Sword Damage", "Sword Speed", "Dash", "Exp",
};

typedef void (*UpgradeFunc)(void);

static Texture2D* ICON_TEXTURES;
static Texture2D* TXT_TEXTURES; // NOTE: must be 52x55 px
static UpgradeFunc* UPGRADE_FUNCS;
static Texture2D cardTexture;
static Texture2D cardFrame;
static bool loaded = false;
static v2 initCardPos = {111, 104};
static i32 cardOffset = 97;
static i32 cardsDrawn = 0;
static Player* plr;
static v2* mouse;
static bool active = false;
static i32 levelsProcessed = 0;
static UpgradeCard** activeCards;

Font alagard;

void upgradeSwordLength() {
    printf("Sword Length upgraded\n");
    ((Sword*)plr->weaponData.weapon)->len += 0.1;
}
void upgradeSwordDamage() { printf("Sword Damage upgraded\n"); }

void grantDash() {
    char text[64];

    if (!plr->canDash) {
        plr->canDash = true;
        plr->dashCooldown = 5;
        snprintf(text, sizeof(text), "PRESS SPACE TO DASH");
    } else {
        snprintf(text, sizeof(text), "COOLDOWN %.2f -> %.2f",
                 plr->dashCooldown, plr->dashCooldown * 0.9);
        plr->dashCooldown *= 0.9;
    }

    printf("%s\n", text);
    i32 len = MeasureTextEx(alagard, text, 20, 1).x;
    v2 pos = {screenWidth / 2.0 - len / 2.0, 230};
    DrawFlashingText(alagard, text, pos, 20, WHITE, 5);
}

void UpgradeCardInit(Player* player, v2* mousePos) {
    if (!loaded) {
        ICON_TEXTURES = malloc(sizeof(Texture2D) * 5);
        ICON_TEXTURES[0] = LoadTexture("assets/images/swordLength.png");
        ICON_TEXTURES[1] = LoadTexture("assets/images/swordDamage.png");
        ICON_TEXTURES[3] = LoadTexture("assets/images/swordSpeed.png");
        ICON_TEXTURES[2] = LoadTexture("assets/images/dash.png");
        ICON_TEXTURES[4] = LoadTexture("assets/images/exp.png");

        TXT_TEXTURES = malloc(sizeof(Texture2D) * 5);
        TXT_TEXTURES[0] = LoadTexture("assets/images/swordLengthTxt.png");
        TXT_TEXTURES[1] = LoadTexture("assets/images/swordDamageTxt.png");
        TXT_TEXTURES[3] = LoadTexture("assets/images/swordSpeedTxt.png");
        TXT_TEXTURES[2] = LoadTexture("assets/images/dashTxt.png");
        TXT_TEXTURES[4] = LoadTexture("assets/images/expTxt.png");

        UPGRADE_FUNCS = malloc(sizeof(UpgradeFunc) * 5);
        UPGRADE_FUNCS[0] = upgradeSwordLength;
        UPGRADE_FUNCS[1] = upgradeSwordDamage;
        UPGRADE_FUNCS[2] = grantDash;

        plr = player;
        mouse = mousePos;
        alagard = LoadFont("assets/fonts/alagard.ttf");
        cardTexture = LoadTexture("assets/images/card.png");
        cardFrame = LoadTexture("assets/images/levelupFrame.png");
        loaded = true;
    }
}

void UpgradeFrameDraw() { DrawTextureV(cardFrame, (v2){99, 49}, WHITE); }

UpgradeCard* UpgradeCardCreate() {
    if (!loaded) {
        fprintf(stderr, "UpgradeCard not initialized\n");
        exit(1);
    }
    UpgradeCard* card = malloc(sizeof(UpgradeCard));
    i32 type = GetRandomValue(0, 2);
    card->type = type;
    card->pos = initCardPos;
    card->hovering = false;
    initCardPos.x += cardOffset;
    cardsDrawn++;
    return card;
}

void UpgradeCardUpdate(UpgradeCard* card) {
    Rect rect = {card->pos.x, card->pos.y, cardTexture.width,
                 cardTexture.height};

    if (CheckCollisionPointRec(*mouse, rect)) {
        card->hovering = true;
    } else {
        card->hovering = false;
    }

    if (card->hovering && IsMouseButtonPressed(0)) {
        UPGRADE_FUNCS[card->type]();
        levelsProcessed++;
        active = false;
    }
}

void UpgradeCardDraw(UpgradeCard* card) {
    Texture2D icon = ICON_TEXTURES[card->type];
    Texture2D txt = TXT_TEXTURES[card->type];
    Color tint = WHITE;

    if (card->hovering) tint = (Color){200, 200, 200, 255};

    DrawTextureV(cardTexture, card->pos, tint);
    DrawTextureV(icon, (v2){card->pos.x + 7, card->pos.y + 30}, tint);
    DrawTextureV(txt, (v2){card->pos.x + 3, card->pos.y + 3}, tint);
}

static void initUI() {
    if (active == true) {
        for (int i = 0; i < 3; i++) {
            free(activeCards[i]);
        }
        free(activeCards);
    } else {
        active = true;
    }
    initCardPos.x = 111;
    activeCards = malloc(sizeof(UpgradeCard) * 3);
    for (int i = 0; i < 3; i++) {
        activeCards[i] = UpgradeCardCreate();
    }
}

void handleLevelupUI() {
    if (!loaded) {
        fprintf(stderr, "UpgradeCard not initialized\n");
        exit(1);
    }

    if (plr->level > levelsProcessed) {
        if (!active) initUI();
        UpgradeFrameDraw();
        for (int i = 0; i < 3; i++) {
            UpgradeCardUpdate(activeCards[i]);
            UpgradeCardDraw(activeCards[i]);
        }
    }
}
