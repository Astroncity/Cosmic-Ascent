#pragma once
#include "defs.h"
#include <functional>
typedef struct RenderData {
    void* obj;
    std::function<void()> render;
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
i32 getActiveRenderers();
