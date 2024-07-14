#pragma once
#include "defs.h"
#include "player.h"

typedef enum UpgradeType {
    UPGRADE_SWORD_LENGTH,
    UPGRADE_SWORD_DAMAGE,
    UPGRADE_DASH,
    // UPGRADE_SWORD_SPEED,
    UPGRADE_EXP,
} UpgradeType;

typedef struct UpgradeCard {
    v2 pos;
    UpgradeType type;
    bool hovering;
} UpgradeCard;

void UpgradeCardInit(Player* player, v2* mousePos);
UpgradeCard* UpgradeCardCreate();
void UpgradeCardDraw(UpgradeCard* card);
void UpgradeFrameDraw();
void UpgradeCardUpdate(UpgradeCard* card);
void handleLevelupUI();
