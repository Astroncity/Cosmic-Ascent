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

    while (curr != NULL) {
        if (curr->obj == obj) {
            obj->markedForDeletion = true;
            return;
        }
        curr = curr->next;
    }
}

void runGameObjects() {
    GameObjectNode* curr = gameObjectHead;
    GameObjectNode* prev = NULL;

    while (curr != NULL) {
        if (curr->obj->markedForDeletion) {
            GameObjectNode* temp = curr;

            if (prev == NULL) {
                // head node is marked for deletion
                gameObjectHead = curr->next;
            } else {
                prev->next = curr->next;
            }

            curr = curr->next;
            free(temp->obj);
            free(temp);
        } else {
            curr->obj->update(curr->obj->obj);
            prev = curr;
            curr = curr->next;
        }
    }
}

GameObject* createGameObject(char* tag, void* obj,
                             Rect (*getCollider)(GameObject*),
                             void (*update)(void*)) {
    GameObject* gameObject = malloc(sizeof(GameObject));
    gameObject->tag = tag;
    gameObject->obj = obj;
    gameObject->getCollider = getCollider;
    gameObject->update = update;
    gameObject->markedForDeletion = false;
    addGameObject(gameObject);
    return gameObject;
}

i32 getActiveGameObjects() {
    GameObjectNode* curr = gameObjectHead;
    i32 count = 0;
    while (curr != NULL) {
        count++;
        curr = curr->next;
    }
    return count;
}
