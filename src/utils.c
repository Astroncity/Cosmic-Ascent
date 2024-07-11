#include "utils.h"
#include <math.h>

f32 lerp(f32 a, f32 b, f32 t) { return a + (b - a) * t; }

f32 lerpAngle(float a, float b, float t) {
    float difference = fmod(b - a + 360.0f, 360.0f);
    if (difference > 180.0f) {
        difference -= 360.0f;
    }
    return a + t * difference;
}

f32 clampAngle(f32 angle) {
    if (angle < 0) {
        angle += 360;
    }
    if (angle > 360) {
        angle -= 360;
    }
    return angle;
}

bool lineLineIntersection(v2 p1, v2 p2, v2 p3, v2 p4) {
    // clang-format off
    f32 x1 = p1.x; f32 y1 = p1.y;
    f32 x2 = p2.x; f32 y2 = p2.y;
    f32 x3 = p3.x; f32 y3 = p3.y;
    f32 x4 = p4.x; f32 y4 = p4.y;
    // clang-format on

    f32 uA = ((x4 - x3) * (y1 - y3) - (y4 - y3) * (x1 - x3)) /
             ((y4 - y3) * (x2 - x1) - (x4 - x3) * (y2 - y1));
    f32 uB = ((x2 - x1) * (y1 - y3) - (y2 - y1) * (x1 - x3)) /
             ((y4 - y3) * (x2 - x1) - (x4 - x3) * (y2 - y1));

    if (uA >= 0 && uA <= 1 && uB >= 0 && uB <= 1) {
        return true;
    }

    return false;
}

v2 RotatePoint(v2 point, v2 origin, f32 angle) {
    f32 s = sinf(angle * DEG2RAD);
    f32 c = cosf(angle * DEG2RAD);

    // Translate point back to origin
    point.x -= origin.x;
    point.y -= origin.y;

    // Rotate point
    f32 xnew = point.x * c - point.y * s;
    f32 ynew = point.x * s + point.y * c;

    // Translate point back
    point.x = xnew + origin.x;
    point.y = ynew + origin.y;

    return point;
}
