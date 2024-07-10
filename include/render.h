#pragma once
#include "defs.h"

typedef struct RenderData {
    void* obj;
    void (*render)(void*);
    i32 priority;
} RenderData;

typedef struct RenderNode RenderNode;
typedef struct RenderNode {
    RenderData data;
    RenderNode* next;
} RenderNode;

void addRender(RenderData data);
void renderAll();
void removeRender(RenderData data);
void renderDebug();
