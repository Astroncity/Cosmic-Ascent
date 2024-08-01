#include "gameobject.hpp"
#include "render.hpp"
#include <stdio.h>
#include <stdlib.h>

std::list<GameObject*> GameObject::gameObjects;

void GameObject::runAll() {
    for (auto it = gameObjects.begin(); it != gameObjects.end();) {
        GameObject* obj = *it;
        if (obj->markedForDeletion) {
            free(obj);
            it = gameObjects.erase(it);
        } else {
            (*it)->update();
            it++;
        }
    }
}

i32 GameObject::getActiveGameObjects() {
    i32 count = 0;

    for (auto it = gameObjects.begin(); it != gameObjects.end(); it++) {
        count++;
    }

    return count;
}

void test() { printf("test\n"); }

GameObject::GameObject(const char* tag, i32 renderPriority)
    : Renderable(renderPriority) {
    this->tag = tag;
    markedForDeletion = false;
    gameObjects.push_back(this);
}

GameObject::GameObject(const char* tag) : Renderable(0) {
    this->tag = tag;
    markedForDeletion = false;
    gameObjects.push_back(this);
}

void GameObject::destroy() {
    removeRender(renderData);
    markedForDeletion = true;
}
