#pragma once
#include "defs.h"

typedef struct GameObject GameObject;
typedef struct GameObject {
    char* tag;
    void* obj;
    Rect (*getCollider)(GameObject* self);
    void (*update)(void* self);
} GameObject;

typedef struct GameObjectNode GameObjectNode;
typedef struct GameObjectNode {
    GameObject* obj;
    GameObjectNode* next;
} GameObjectNode;

extern GameObjectNode* gameObjectHead;

void runGameObjects();
void removeGameObject(GameObject* obj);
GameObject* createGameObject(char* tag, void* obj,
                             Rect (*getCollider)(GameObject*),
                             void (*update)(void*));
i32 getActiveGameObjects();
