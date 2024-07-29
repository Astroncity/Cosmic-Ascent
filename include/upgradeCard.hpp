#pragma once
#include "defs.h"
#include <vector>

typedef enum UpgradeType {
    UPGRADE_SWORD_LENGTH,
    UPGRADE_SWORD_DAMAGE,
    UPGRADE_DASH,
    // UPGRADE_SWORD_SPEED,
    UPGRADE_EXP,
} UpgradeType;

/*typedef struct UpgradeCard {
    v2 pos;
    UpgradeType type;
    bool hovering;
} UpgradeCard;

UpgradeCard* UpgradeCardCreate();
void UpgradeCardDraw(UpgradeCard* card);
void UpgradeFrameDraw();
void UpgradeCardUpdate(UpgradeCard* card);
void handleLevelupUI();*/

class UpgradeCard {
  public:
    v2 pos;
    UpgradeType type;
    bool hovering;
    static i32 levelsProcessed;
    static bool active;
    static std::vector<UpgradeCard*> activeCards;

    UpgradeCard();
    void draw();
    void update();
    static void handleLevelupUI();
    static void drawFrame();

  private:
    static void initUI();
};
