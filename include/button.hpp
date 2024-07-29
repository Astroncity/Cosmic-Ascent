#pragma once
#include "defs.h"
#include "gameobject.hpp"

/*typedef struct Button {
    Texture2D texture;
    Rect bounds;
    void (*onClick)(void);
    bool destroyed;
    bool hovered;
} Button;

Button* ButtonCreate(v2 pos, Texture2D texture, void (*onClick)(void));
void ButtonDestroy(Button* button);*/

class Button : public GameObject {
  public:
    Texture2D texture;
    Rectangle bounds;
    void (*onClick)(void);
    bool destroyed;
    bool hovered;

    Button(v2 pos, Texture2D texture, void (*onClick)(void));
    void destroy() override;
    void render() override;
    void update() override;
    Rect getCollider() override;
};
