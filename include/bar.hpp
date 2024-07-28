#pragma once
#include "defs.h"

/*typedef struct Bar {
    Texture2D barTexture;
    Texture2D background;
    Rect rect;
    f32 value;
    f32 currValue;
    f32 oldValue;
    f32 maxValue;
} Bar;

Bar* BarCreate(f32 x, f32 y, f32 maxValue, bool large);
void BarRender(Bar* bar, Color tint, bool drawBackground);*/

class Bar {
  private:
    Texture2D barTexture;
    Texture2D background;
    Rectangle rect;
    f32 currValue;
    f32 oldValue;

  public:
    f32 value;
    f32 maxValue;
    Bar(f32 x, f32 y, f32 maxValue, bool large);
    void render(Color tint, bool drawBackground);
    void updatePos(f32 x, f32 y);
};
