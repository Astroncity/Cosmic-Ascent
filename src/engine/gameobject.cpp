#include "gameobject.hpp"
#include <stdlib.h>

std::list<GameObject*> GameObject::gameObjects;

void GameObject::runAll() {
    for (auto it = gameObjects.begin(); it != gameObjects.end(); it++) {
        (*it)->update();

        if ((*it)->markedForDeletion) {
            it = gameObjects.erase(it);
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
