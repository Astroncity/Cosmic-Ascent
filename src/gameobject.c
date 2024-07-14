#include "gameobject.h"
#include <stdlib.h>

GameObjectNode* gameObjectHead = NULL;

void addGameObject(GameObject* obj) {
    GameObjectNode* new = malloc(sizeof(GameObjectNode));
    new->obj = obj;
    new->next = NULL;

    if (gameObjectHead == NULL) {
        gameObjectHead = new;
        return;
    }

    GameObjectNode* curr = gameObjectHead;
    while (curr->next != NULL) {
        curr = curr->next;
    }

    curr->next = new;
}

void removeGameObject(GameObject* obj) {
    GameObjectNode* curr = gameObjectHead;
    GameObjectNode* prev = NULL;

    while (curr != NULL) {
        if (curr->obj == obj) {
            if (prev == NULL) {
                gameObjectHead = curr->next;
            } else {
                prev->next = curr->next;
            }

            GameObjectNode* temp = curr;
            curr = curr->next;
            free(temp);
        } else {
            prev = curr;
            curr = curr->next;
        }
    }
}

void runGameObjects() {}

GameObject* createGameObject(char* tag, void* obj,
                             Rect (*getCollider)(GameObject*)) {
    GameObject* gameObject = malloc(sizeof(GameObject));
    gameObject->tag = tag;
    gameObject->obj = obj;
    gameObject->getCollider = getCollider;
    addGameObject(gameObject);
    return gameObject;
}
