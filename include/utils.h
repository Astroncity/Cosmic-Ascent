#pragma once
#include "defs.h"

f32 lerp(f32 a, f32 b, f32 t);
f32 lerpAngle(f32 a, f32 b, f32 t);
f32 clampAngle(f32 angle);
bool lineLineIntersection(v2 p1, v2 p2, v2 p3, v2 p4);
v2 RotatePoint(v2 point, v2 origin, f32 angle);
