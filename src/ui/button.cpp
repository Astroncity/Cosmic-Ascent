#include "button.hpp"
#include "globals.hpp"

#define RENDER_PRIORITY 10

void Button::render() {
    Color tint = WHITE;
    if (hovered) {
        tint = (Color){150, 150, 150, 255};
    }

    DrawTexture(texture, bounds.x, bounds.y, tint);
}

Rect Button::getCollider() { return {}; }

void Button::destroy() {
    removeRender(renderData);
    markedForDeletion = true;
}

void Button::update() {
    if (destroyed) {
        destroy();
        return;
    }

    if (CheckCollisionPointRec(mouse, bounds)) {
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            onClick();
        }
        hovered = true;
    } else {
        hovered = false;
    }
}

Button::Button(v2 pos, Texture2D texture, void (*onClick)(void))
    : GameObject("Button", RENDER_PRIORITY) {
    this->texture = texture;
    this->bounds = (Rect){pos.x, pos.y, texture.width, texture.height};
    this->onClick = onClick;
    this->destroyed = false;
    this->hovered = false;
}
