#pragma once
#include "raylib.h"

typedef struct GameObject GameObject;
typedef struct GameObject {
    char* tag;
    void* obj;
    Rectangle (*getCollider)(GameObject* self);
} GameObject;

GameObject* createGameObject(char* tag, void* obj,
                             Rectangle (*getCollider)(GameObject*));

typedef struct GameObjectNode GameObjectNode;
typedef struct GameObjectNode {
    GameObject* obj;
    GameObjectNode* next;
} GameObjectNode;

extern GameObjectNode* gameObjectHead;

void runGameObjects();
void removeGameObject(GameObject* obj);
