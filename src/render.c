#include "render.h"
#include <stdio.h>
#include <stdlib.h>

RenderNode* renderHead = NULL;

void addRender(RenderData data) {
    RenderNode* new = (RenderNode*)malloc(sizeof(RenderNode));
    new->data = data;
    new->next = NULL;

    // If the list is empty or the new node should be the new head
    if (renderHead == NULL || renderHead->data.priority > data.priority) {
        new->next = renderHead;
        renderHead = new;
        return;
    }

    RenderNode* curr = renderHead;
    // Traverse the list to find the correct position
    while (curr->next != NULL && curr->next->data.priority <= data.priority) {
        curr = curr->next;
    }

    // Insert the new node
    new->next = curr->next;
    curr->next = new;
}

void renderAll() {
    RenderNode* curr = renderHead;
    while (curr != NULL) {
        curr->data.render(curr->data.obj);
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
