#include "sword.h"
#include "player.h"
#include "raylib.h"
#include "render.h"
#include "task.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

static i32 distanceFromPlr = 25;
static f32 angle;
static f32 previousAngle;
static f32 angleDelta;
bool mouseControl = true;

static struct SpinData {
    f32 start;
    f32 runTime;
    f32* angle;
} SpinData;

struct SpinData spinData = {
    .start = 0,
    .runTime = 1,
    .angle = &angle,
};

static f32 getAngleToMouse(Sword* sword) {
    Vector2 diff;
    diff.x = sword->mouse->x - sword->owner->rect.x;
    diff.y = sword->mouse->y - sword->owner->rect.y;
    return atan2(diff.y, diff.x) * RAD2DEG;
}

f32 lerp(f32 a, f32 b, f32 t) { return a + (b - a) * t; }

float lerpAngle(float a, float b, float t) {
    float difference = fmod(b - a + 360.0f, 360.0f);
    if (difference > 180.0f) {
        difference -= 360.0f;
    }
    return a + t * difference;
}

f32 clampAngle(f32 angle) {
    if (angle < 0) {
        angle += 360;
    }
    if (angle > 360) {
        angle -= 360;
    }
    return angle;
}

void spinTask(TASK_PARAMS) {
    // ends mouse control and spins sword for 1 sec
    struct SpinData* data = (struct SpinData*)taskData;
    mouseControl = false;

    bool ended = GetTime() >= data->start + data->runTime;

    if (ended) {
        mouseControl = true;
        task->setForDeletion = true;
    }

    *data->angle += 360 * 2 * GetFrameTime();
}

static void control(Sword* sword) {
    f32 sens = 5 * GetFrameTime();
    f32 angleToMouse = getAngleToMouse(sword);
    angle = lerpAngle(angle, angleToMouse, sens);
    angle = clampAngle(angle);
}

static void use(void* swordP) {
    Sword* sword = (Sword*)swordP;
    sword->rect.x = sword->owner->rect.x;
    sword->rect.y = sword->owner->rect.y;

    if (mouseControl) control(sword);

    angleDelta = angle - previousAngle;
    printf("speed: %f\n", fabs(angleDelta));

    sword->rect.x += cos(DEG2RAD * angle) * distanceFromPlr;
    sword->rect.y += sin(DEG2RAD * angle) * distanceFromPlr;

    if (IsMouseButtonPressed(0) && mouseControl) {
        printf("Sword used\n");
        spinData.start = GetTime();
        createTask(&spinData, spinTask);
    }

    previousAngle = angle;
}

static void render(void* swordP) {
    Sword* sword = (Sword*)swordP;
    Rectangle src = {0, 0, sword->texture.width, sword->texture.height};
    Rectangle dest = sword->rect;
    dest.width = sword->texture.width * 1.25;
    dest.height = sword->texture.height * 1.25;
    Vector2 org = {dest.width / 2, dest.height / 2};

    // rotate to face mouse, sword is pointing at top right in image

    DrawTexturePro(sword->texture, src, dest, org, clampAngle(angle + 45),
                   sword->cl);
}

Sword* createSword(Player* owner, Vector2* mouse, Color cl) {
    Sword* sword = malloc(sizeof(Sword));
    sword->texture = LoadTexture("assets/images/sword.png");
    SetTextureFilter(sword->texture, TEXTURE_FILTER_POINT);
    sword->cl = cl;
    sword->render = render;
    sword->use = use;
    sword->owner = owner;
    sword->mouse = mouse;
    addRender((RenderData){sword, render, 0});
    return sword;
}
