#pragma once
#include "defs.h"
#define MAX(a, v) ((a) > (v) ? (a) : (v))
#define MIN(a, v) ((a) < (v) ? (a) : (v))

f32 lerp(f32 a, f32 b, f32 t);
f32 lerpAngle(f32 a, f32 b, f32 t);
f32 clampAngle(f32 angle);
bool lineLineIntersection(v2 p1, v2 p2, v2 p3, v2 p4);
v2 RotatePoint(v2 point, v2 origin, f32 angle);

// used to be in main
v2 v2Clamp(v2 vec, v2 min, v2 max);
void drawScaledWindow(RenderTexture2D target, f32 sw, f32 sh, f32 scale);
v2 getScreenMousePos(v2* mouse, f32 scale, i32 sw, i32 sh);
f32 getAngleToPoint(v2 a, v2 b);
