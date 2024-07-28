#include "sword.h"
#include "gameobject.h"
#include "globals.h"
#include "player.h"
#include "raylib.h"
#include "rock.hpp"
#include "slime.hpp"
#include "slimeGhoul.hpp"
#include "task.h"
#include "utils.h"
#include <math.h>
#include <stdio.h>
#include <string.h>

#define ATTACK_SPIN 0
#define RECOIL_SPIN 1

static i32 distanceFromPlr = 25;

static Texture2D swordTex;
static Texture2D swordEffect;
static bool loadedTexture = false;
static f32 defWidth;
static f32 defHeight;

v2 swordOrg;

SpinData spinData = {.start = 0,
                     .runTime = 1,
                     .direction = 1,
                     .type = ATTACK_SPIN,
                     .speed = 360 * 4,
                     .sword = NULL};

SpinData recoilData = {.start = 0,
                       .runTime = 0.5,
                       .direction = -1,
                       .type = RECOIL_SPIN,
                       .speed = 360 * 3,
                       .sword = NULL};

static v2* getColliderLines(Sword* sword) {
    Rect r = sword->rect;
    r.width = 23 * sword->len;
    r.height = 5 * sword->len;
    r.x -= r.width / 2;
    r.y -= r.height / 2;
    v2 topLeft = {r.x, r.y};
    v2 topRight = {r.x + r.width, r.y};
    v2 botLeft = {r.x, r.y + r.height};
    v2 botRight = {r.x + r.width, r.y + r.height};

    v2 rotOrg = {sword->rect.x, sword->rect.y};

    topLeft = RotatePoint(topLeft, rotOrg, sword->angle);
    topRight = RotatePoint(topRight, rotOrg, sword->angle);
    botLeft = RotatePoint(botLeft, rotOrg, sword->angle);
    botRight = RotatePoint(botRight, rotOrg, sword->angle);

    v2* lines = (v2*)malloc(sizeof(v2) * 8);
    lines[0] = topLeft;
    lines[1] = topRight;
    lines[2] = topRight;
    lines[3] = botRight;
    lines[4] = botRight;
    lines[5] = botLeft;
    lines[6] = botLeft;
    lines[7] = topLeft;

    return lines;
}

static v2* getAxisRectLines(Rect r) {
    v2 topLeft = {r.x, r.y};
    v2 topRight = {r.x + r.width, r.y};
    v2 botLeft = {r.x, r.y + r.height};
    v2 botRight = {r.x + r.width, r.y + r.height};

    v2* lines = (v2*)malloc(sizeof(v2) * 8);
    lines[0] = topLeft;
    lines[1] = topRight;
    lines[2] = topRight;
    lines[3] = botRight;
    lines[4] = botRight;
    lines[5] = botLeft;
    lines[6] = botLeft;
    lines[7] = topLeft;

    return lines;
}

static void spinTask(TASK_PARAMS) {
    // ends mouse control and spins sword for 1 sec
    SpinData* data = (SpinData*)taskData;
    Sword* sword = data->sword;

    bool ended = GetTime() >= data->start + data->runTime;
    if (ended || (sword->underRecoil && data->type != RECOIL_SPIN)) {
        sword->mouseControl = true;
        task->setForDeletion = true;

        if (data->type == RECOIL_SPIN) {
            sword->underRecoil = false;
        }
        free(data);
        return;
    }

    if (data->type == RECOIL_SPIN) {
        sword->underRecoil = true;
    }

    sword->mouseControl = false;
    sword->angle += data->speed * GetFrameTime() * data->direction;
    sword->angle = clampAngle(sword->angle);
}

static bool onCollision(Sword* sword, GameObject* other) {
    sword->invulnerableTimer = 0.25;
    if (strcmp(other->tag, "rock") == 0) {
        Rock* rock = (Rock*)other->obj;
        return rock->hit(rock);
    }
    if (strcmp(other->tag, "slime") == 0) {
        Slime* slime = (Slime*)other->obj;
        slime->health -= 0.25 * (1 + fabs(sword->angleDelta));
    }
    if (strcmp(other->tag, "slime ghoul") == 0) {
        SlimeGhoul* slime = (SlimeGhoul*)other->obj;
        slime->health -= 0.25 * (1 + fabs(sword->angleDelta));
    }
    return false;
}

static bool handleCollision(Sword* sword) {
    // assume other collider is axis aligned
    // if (sword->invulnerableTimer > 0) return false;
    if (sword->angleDelta < 1) return false;
    v2* swordLines = getColliderLines(sword);
    bool hit = false;
    bool rmObj = false;
    i32 iterations = 0;

    GameObjectNode* curr = gameObjectHead;

    while (curr != NULL) {
        v2* otherLines =
            getAxisRectLines(curr->obj->getCollider(curr->obj));
        for (int i = 0; i < 8; i += 2) {
            for (int j = 0; j < 8; j += 2) {
                if (lineLineIntersection(swordLines[i], swordLines[i + 1],
                                         otherLines[i],
                                         otherLines[i + 1])) {
                    rmObj = onCollision(sword, curr->obj);
                    hit = true;
                    goto main_loop;
                }
            }
        }
    main_loop:
        if (rmObj) {
            curr = gameObjectHead;
            for (i32 i = 0; i < iterations; i++) {
                curr = curr->next;
            }
        } else {
            curr = curr->next;
        }
        free(otherLines);
        iterations++;
    }

    // cleanup:
    free(swordLines);
    return hit;
}

static void DrawSwordColliderLines(Sword* sword) {
    // Get the lines representing the rotated sword's rectangle
    v2* lines = getColliderLines(sword);

    // Draw each line
    for (int i = 0; i < 8; i += 2) {
        DrawLineEx(lines[i], lines[i + 1], 1, RED);
    }

    free(lines);
}

static void control(Sword* sword) {
    v2 swordPos = {sword->rect.x, sword->rect.y};
    f32 angleToMouse = getAngleToPoint(swordPos, mouse);
    f32 sens = 5 * 2 * GetFrameTime();
    sword->angle = lerpAngle(sword->angle, angleToMouse, sens);
    sword->angle = clampAngle(sword->angle);
}

static void use(void* swordP) {
    Sword* sword = (Sword*)swordP;
    sword->invulnerableTimer -= GetFrameTime();
    sword->rect.x = sword->owner->rect.x;
    sword->rect.y = sword->owner->rect.y;
    sword->rect.width = defWidth * sword->len;
    sword->rect.height = defHeight * sword->len;

    if (sword->mouseControl) control(sword);
    sword->rect.x += cos(DEG2RAD * sword->angle) * distanceFromPlr *
                     (1 + sword->len / 5);
    sword->rect.y += sin(DEG2RAD * sword->angle) * distanceFromPlr *
                     (1 + sword->len / 5);
    if (handleCollision(sword)) {
        sword->rect.x = sword->prevPos.x;
        sword->rect.y = sword->prevPos.y;
        sword->angle = sword->previousAngle;
    }

    sword->prevPos = (v2){sword->rect.x, sword->rect.y};
    sword->angleDelta = sword->angle - sword->previousAngle;
    sword->previousAngle = sword->angle;
    if (IsMouseButtonPressed(0) && sword->mouseControl) {
        SpinData* newData = (SpinData*)malloc(sizeof(SpinData));
        *newData = spinData;
        newData->start = GetTime();
        newData->sword = sword;
        createTask("sword spin", newData, spinTask);
    }
}

static void render(void* swordP) {
    Sword* sword = (Sword*)swordP;
    Rect src = {0, 0, (f32)swordTex.width, (f32)swordTex.height};

    Rect dest = sword->rect;
    swordOrg = (v2){dest.width / 2, dest.height / 2};

    DrawTexturePro(swordTex, src, dest, swordOrg,
                   clampAngle(sword->angle + 45), sword->cl);

    // draw collider
    if (true) DrawSwordColliderLines(sword);
}

Sword* createSword(Player* owner, v2* mouse, Color cl) {
    Sword* sword = (Sword*)malloc(sizeof(Sword));
    if (!loadedTexture) {
        swordTex = LoadTexture("assets/images/sword.png");
        swordEffect = LoadTexture("assets/images/attackEff.png");
        SetTextureFilter(swordTex, TEXTURE_FILTER_POINT);
        defWidth = swordTex.width * 1.25;
        defHeight = swordTex.height * 1.25;
    }
    sword->cl = cl;
    sword->render = render;
    sword->use = use;
    sword->owner = owner;
    sword->mouse = mouse;
    sword->rect.width = swordTex.width * 1.25;
    sword->rect.height = swordTex.height * 1.25;
    sword->mouseControl = true;
    sword->invulnerableTimer = 0;
    sword->len = 1;
    return sword;
}
