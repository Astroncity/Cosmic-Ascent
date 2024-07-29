#include "render.hpp"
#include <stdio.h>
#include <stdlib.h>

RenderNode* renderHead = NULL;

void addRender(RenderData data) {
    // RenderNode* rend = (RenderNode*)malloc(sizeof(RenderNode));
    RenderNode* rend = new RenderNode();
    rend->data = data;
    rend->next = NULL;

    // If the list is empty or the rend node should be the new head
    if (renderHead == NULL || renderHead->data.priority > data.priority) {
        rend->next = renderHead;
        renderHead = rend;
        return;
    }

    RenderNode* curr = renderHead;
    // Traverse the list to find the correct position
    while (curr->next != NULL &&
           curr->next->data.priority <= data.priority) {
        curr = curr->next;
    }

    // Insert the rend node
    rend->next = curr->next;
    curr->next = rend;
}

bool compareRenderData(RenderData a, RenderData b) {
    if (a.priority == b.priority &&
        a.render.target<void()>() == b.render.target<void()>() &&
        a.render.target_type() == b.render.target_type() &&
        a.obj == b.obj) {
        return true;
    }
    return false;
}

void removeRender(RenderData data) {
    RenderNode* curr = renderHead;
    RenderNode* prev = NULL;

    while (curr != NULL) {
        if (compareRenderData(data, curr->data)) {
            if (prev == NULL) {
                renderHead = curr->next;
            } else {
                prev->next = curr->next;
            }

            RenderNode* temp = curr;
            curr = curr->next;
            delete temp;
        } else {
            prev = curr;
            curr = curr->next;
        }
    }
}

void renderAll() {
    RenderNode* curr = renderHead;
    while (curr != NULL) {
        curr->data.render();
        curr = curr->next;
    }
}

void renderDebug() {
    RenderNode* curr = renderHead;
    printf("[RENDER QUEUE]");
    while (curr != NULL) {
        printf(" %d ->", curr->data.priority);
        curr = curr->next;
    }
}

i32 getActiveRenderers() {
    i32 count = 0;
    RenderNode* curr = renderHead;
    while (curr != NULL) {
        count++;
        curr = curr->next;
    }
    return count;
}
