#include "button.h"
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

Button* ButtonCreate(v2 pos, Texture2D texture, void (*onClick)(void)) {
    Button* button = malloc(sizeof(Button));
    button->texture = texture;
    button->bounds = (Rect){pos.x, pos.y, texture.width, texture.height};
    button->onClick = onClick;
    button->destroyed = false;
    button->hovered = false;
    addRender((RenderData){button, render, RENDER_PRIORITY});
    createTask("button update", button, update);
    return button;
}
void ButtonDestroy(Button* button) { button->destroyed = true; }
