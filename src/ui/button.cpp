#include "button.hpp"
#include "globals.h"
#include "render.h"
#include "task.h"

#define RENDER_PRIORITY 10

static void render(void* buttonP) {
    Button* button = (Button*)buttonP;
    Color tint = WHITE;
    if (button->hovered) {
        tint = (Color){150, 150, 150, 255};
    }

    DrawTexture(button->texture, button->bounds.x, button->bounds.y, tint);
}

static void destroy(Button* button) {
    removeRender((RenderData){button, render, RENDER_PRIORITY});
    free(button);
}

static void update(TASK_PARAMS) {
    Button* button = (Button*)taskData;
    if (button->destroyed) {
        task->setForDeletion = true;
        destroy(button);
        return;
    }

    if (CheckCollisionPointRec(mouse, button->bounds)) {
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            button->onClick();
        }
        button->hovered = true;
    } else {
        button->hovered = false;
    }
}

Button::Button(v2 pos, Texture2D texture, void (*onClick)(void)) {
    this->texture = texture;
    this->bounds = (Rect){pos.x, pos.y, texture.width, texture.height};
    this->onClick = onClick;
    this->destroyed = false;
    this->hovered = false;
    addRender((RenderData){this, render, RENDER_PRIORITY});
    createTask("button update", this, update);
}
void Button::destroy() { destroyed = true; }
