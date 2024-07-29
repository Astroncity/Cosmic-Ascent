#pragma once
#include "defs.h"

void DrawText3D(Font font, const char* text, i32 x, i32 y, i32 size,
                Color color);

void DrawFlashingText(Font font, const char* text, v2 pos, i32 size,
                      Color color, f32 duration);
