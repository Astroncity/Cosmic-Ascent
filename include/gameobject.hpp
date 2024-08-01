#pragma once
#include "defs.h"
#include "render.hpp"
#include <list>

/*typedef struct GameObject GameObject;
typedef struct GameObject {
    const char* tag;
    void* obj;
    Rect (*getCollider)(GameObject* self);
    void (*update)(void* self);
    bool markedForDeletion;
} GameObject;

typedef struct GameObjectNode GameObjectNode;
typedef struct GameObjectNode {
    GameObject* obj;
    GameObjectNode* next;
} GameObjectNode;

extern GameObjectNode* gameObjectHead;

void runGameObjects();
void removeGameObject(GameObject* obj);
GameObject* createGameObject(const char* tag, void* obj,
                             Rect (*getCollider)(GameObject*),
                             void (*update)(void*));
i32 getActiveGameObjects();*/

class GameObject : public Renderable {
  public:
    const char* tag;
    bool markedForDeletion;

    GameObject(const char* tag, i32 renderPriority);
    GameObject(const char* tag);

    virtual Rect getCollider() = 0;
    virtual void update() = 0;
    virtual void destroy() = 0;

    static void runAll();
    static std::list<GameObject*> gameObjects;
    static i32 getActiveGameObjects();
};
