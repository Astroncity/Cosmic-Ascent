#include "upgradeCard.hpp"
#include "globals.hpp"
#include "textEffects.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <vector>

const char* UPGRADE_NAMES[] = {
    "Sword Length", "Sword Damage", "Sword Speed", "Dash", "Exp",
};

typedef void (*UpgradeFunc)(void);
std::vector<UpgradeCard*> UpgradeCard::activeCards;
i32 UpgradeCard::levelsProcessed = 0;
bool UpgradeCard::active = false;

static Texture2D* ICON_TEXTURES;
static Texture2D* TXT_TEXTURES; // NOTE: must be 52x55 px
static UpgradeFunc* UPGRADE_FUNCS;
static Texture2D cardTexture;
static Texture2D cardFrame;
static bool loaded = false;
static v2 initCardPos = {111, 104};
static i32 cardOffset = 97;

Font alagard;

void upgradeSwordLength() {
    printf("Sword Length upgraded\n");
    ((Sword*)player->weaponData.weapon)->len += 0.1;
}
void upgradeSwordDamage() { printf("Sword Damage upgraded\n"); }

void grantDash() {
    char text[64];

    if (!player->canDash) {
        player->canDash = true;
        player->dashCooldown = 5;
        snprintf(text, sizeof(text), "PRESS SPACE TO DASH");
    } else {
        snprintf(text, sizeof(text), "COOLDOWN %.2f -> %.2f",
                 player->dashCooldown, player->dashCooldown * 0.9);
        player->dashCooldown *= 0.9;
    }

    printf("%s\n", text);
    i32 len = MeasureTextEx(alagard, text, 20, 1).x;
    v2 pos = {screenWidth / 2.0 - len / 2.0, 230};
    DrawFlashingText(alagard, text, pos, 20, WHITE, 5);
}

void UpgradeCardInit() {
    if (!loaded) {
        ICON_TEXTURES = (Texture2D*)malloc(sizeof(Texture2D) * 5);
        ICON_TEXTURES[0] =
            LoadTexture("assets/images/upgrades/swordLength.png");
        ICON_TEXTURES[1] =
            LoadTexture("assets/images/upgrades/swordDamage.png");
        ICON_TEXTURES[3] =
            LoadTexture("assets/images/upgrades/swordSpeed.png");
        ICON_TEXTURES[2] = LoadTexture("assets/images/upgrades/dash.png");
        ICON_TEXTURES[4] = LoadTexture("assets/images/upgrades/exp.png");

        TXT_TEXTURES = (Texture2D*)malloc(sizeof(Texture2D) * 5);
        TXT_TEXTURES[0] =
            LoadTexture("assets/images/upgrades/swordLengthTxt.png");
        TXT_TEXTURES[1] =
            LoadTexture("assets/images/upgrades/swordDamageTxt.png");
        TXT_TEXTURES[3] =
            LoadTexture("assets/images/upgrades/swordSpeedTxt.png");
        TXT_TEXTURES[2] = LoadTexture("assets/images/upgrades/dashTxt.png");
        TXT_TEXTURES[4] = LoadTexture("assets/images/upgrades/expTxt.png");

        UPGRADE_FUNCS = (UpgradeFunc*)malloc(sizeof(UpgradeFunc) * 5);
        UPGRADE_FUNCS[0] = upgradeSwordLength;
        UPGRADE_FUNCS[1] = upgradeSwordDamage;
        UPGRADE_FUNCS[2] = grantDash;

        alagard = LoadFont("assets/fonts/alagard.ttf");
        cardTexture = LoadTexture("assets/images/upgrades/card.png");
        cardFrame = LoadTexture("assets/images/upgrades/levelupFrame.png");
        loaded = true;
    }
}

void UpgradeCard::drawFrame() { DrawTextureV(cardFrame, {99, 49}, WHITE); }

UpgradeCard::UpgradeCard() {
    UpgradeCardInit();
    type = (UpgradeType)GetRandomValue(0, 2);
    pos = initCardPos;
    hovering = false;
    initCardPos.x += cardOffset;
}

void UpgradeCard::update() {
    Rect rect = {pos.x, pos.y, cardTexture.width, cardTexture.height};

    if (CheckCollisionPointRec(mouse, rect)) {
        hovering = true;
    } else {
        hovering = false;
    }

    if (hovering && IsMouseButtonPressed(0)) {
        UPGRADE_FUNCS[type]();
        levelsProcessed++;
        active = false;
    }
}

void UpgradeCard::draw() {
    Texture2D icon = ICON_TEXTURES[type];
    Texture2D txt = TXT_TEXTURES[type];
    Color tint = WHITE;

    if (hovering) tint = (Color){200, 200, 200, 255};

    DrawTextureV(cardTexture, pos, tint);
    DrawTextureV(icon, (v2){pos.x + 7, pos.y + 30}, tint);
    DrawTextureV(txt, (v2){pos.x + 3, pos.y + 3}, tint);
}

void UpgradeCard::initUI() {
    if (active == true) {
        for (auto p : activeCards) {
            delete p;
        }
        activeCards.clear();
    }

    initCardPos.x = 111;
    for (int i = 0; i < 3; i++) {
        activeCards.push_back(new UpgradeCard());
    }
}

void UpgradeCard::handleLevelupUI() {
    if (player->level > levelsProcessed) {
        if (!active) initUI();
        drawFrame();
        for (int i = 0; i < 3; i++) {
            activeCards[i]->update();
            activeCards[i]->draw();
        }
    }
}
